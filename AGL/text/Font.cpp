#include "Font.h"

#include <string.h>
#include <memory>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_OUTLINE_H

#include <msdfgen.h>

#include "rect.h"

// Copy-pasted from the following URL (with edits) because why rewrite the code
// for someone else's interface.
// https://github.com/Chlumsky/msdfgen/blob/master/ext/import-font.cpp

struct FtContext {
    msdfgen::Point2 position;
    msdfgen::Shape* shape;
    msdfgen::Contour* contour;
};

static msdfgen::Point2 ftPoint2(const FT_Vector& vector) {
    return msdfgen::Point2(vector.x / 64.0, vector.y / 64.0);
}

static int ftMoveTo(const FT_Vector* to, void* user) {
    FtContext *context = reinterpret_cast<FtContext*>(user);
    context->contour = &context->shape->addContour();
    context->position = ftPoint2(*to);
    return 0;
}

static int ftLineTo(const FT_Vector* to, void* user) {
    FtContext *context = reinterpret_cast<FtContext*>(user);
    context->contour->addEdge(
      new msdfgen::LinearSegment(context->position, ftPoint2(*to)));
    context->position = ftPoint2(*to);
    return 0;
}

static int ftConicTo(
    const FT_Vector* control, const FT_Vector* to, void* user) {
  FtContext *context = reinterpret_cast<FtContext*>(user);
  context->contour->addEdge(new msdfgen::QuadraticSegment(
    context->position, ftPoint2(*control), ftPoint2(*to)));
  context->position = ftPoint2(*to);
  return 0;
}

static int ftCubicTo(
    const FT_Vector* control1, const FT_Vector* control2,
    const FT_Vector* to, void* user) {
  FtContext *context = reinterpret_cast<FtContext*>(user);
  context->contour->addEdge(new msdfgen::CubicSegment(
    context->position,
    ftPoint2(*control1), ftPoint2(*control2), ftPoint2(*to)));
  context->position = ftPoint2(*to);
  return 0;
}

static FT_Outline_Funcs_ myOutlineFuncs = {
  ftMoveTo,
  ftLineTo,
  ftConicTo,
  ftCubicTo,
  0,
  0
};

namespace agl {
  const char* ftErrorToString(FT_Error code) {
    // thx https://stackoverflow.com/questions/31161284/how-can-i-get-the-corresponding-error-string-from-an-ft-error-code#32665249
    #undef __FTERRORS_H__
    #define FT_ERRORDEF( e, v, s )  case e: return s;
    #define FT_ERROR_START_LIST     switch (code) {
    #define FT_ERROR_END_LIST       }
    #include FT_ERRORS_H
    return "(Unknown error)";
  }

  struct Node {
    std::unique_ptr<Node> child[2];
    TRect<uint32_t> bounds;
    size_t glyph;
    Node* insert(size_t w, size_t h) {
      if (child[0] != nullptr) {
        Node* nn = child[0]->insert(w, h);
        if (nn != nullptr) return nn;
        return child[1]->insert(w, h);
      }
      if (glyph != -1U) return nullptr;
      if (w > bounds.right - bounds.left ||
          h > bounds.bottom - bounds.top) {
        return nullptr;
      }
      if (w == bounds.right - bounds.left &&
          h == bounds.bottom - bounds.top) {
        return this;
      }
      child[0] = std::make_unique<Node>();
      child[1] = std::make_unique<Node>();
      uint32_t dw = bounds.right - bounds.left - w;
      uint32_t dh = bounds.bottom - bounds.top - h;
      if (dw > dh) {
        child[0]->bounds = {
          bounds.left, bounds.top, bounds.left + dw, bounds.bottom
        };
        child[1]->bounds = {
          bounds.left + dw, bounds.top, bounds.right, bounds.bottom
        };
      } else {
        child[0]->bounds = {
          bounds.left, bounds.top, bounds.right, bounds.top + dh
        };
        child[1]->bounds = {
          bounds.left, bounds.top + dh, bounds.right, bounds.bottom
        };
      }
      return child[0]->insert(w, h);
    }
  };

  Font::Font(FT_Library ftl, const char* filename, size_t size) {
    FT_Face face = nullptr;
    FT_Error error = FT_New_Face(ftl, filename, 0, &face);
    if (error != 0) throw FTException(error);
    uint8_t atlas[ATLAS_SIZE][ATLAS_SIZE][4];
    Node n;
    n.bounds = { 0, 0, ATLAS_SIZE, ATLAS_SIZE };
    auto stash = [this, atlas]() {
      texs.emplace_back(ATLAS_SIZE, ATLAS_SIZE, (const uint8_t*) atlas);
    };
    // Texture packing algorithm from 
    // http://blackpawn.com/texts/lightmaps/default.html
    for (unsigned i = 0; i < (size_t) face->num_glyphs; ++i) {
      // Load the glyph
      error = FT_Load_Glyph(face, i, FT_LOAD_NO_SCALE | FT_LOAD_NO_BITMAP);
      if (error != 0) goto rek;
      msdfgen::Shape output;
      output.contours.clear();
      output.inverseYAxis = false;
      FtContext ctx;
      ctx.shape = &output;
      error = FT_Outline_Decompose(
        &(face->glyph->outline),
        &myOutlineFuncs,
        &ctx
      );
      if (error != 0) goto rek;
      // Generate the MSDF
      output.normalize();
      msdfgen::edgeColoringSimple(output, 3.0);
      size_t margin = size / 8;
      msdfgen::Bitmap<msdfgen::FloatRGB> bm(
        (size * face->glyph->linearHoriAdvance >> 16) + 2 * margin,
        size + 2 * margin
      );
      msdfgen::generateMSDF(
        bm,
        output,
        margin,
        1.0,
        msdfgen::Vector2(margin, margin)
      );
      // Copy the MSDF into the texture
      Node* where = n.insert(bm.width(), bm.height());
      if (where == nullptr) {
        stash();
        n = Node();
        n.bounds = { 0, 0, ATLAS_SIZE, ATLAS_SIZE };
        where = n.insert(bm.width(), bm.height());
        if (where == nullptr) throw "really?";
      }
      for (uint32_t y = where->bounds.top; y < where->bounds.bottom; ++y) {
        for (uint32_t x = where->bounds.left; x < where->bounds.right; ++x) {
          int tx = x - where->bounds.left;
          int ty = y - where->bounds.right;
          auto col = bm(tx, ty);
          atlas[y][x][0] = (uint8_t) (col.r * 255);
          atlas[y][x][1] = (uint8_t) (col.g * 255);
          atlas[y][x][2] = (uint8_t) (col.b * 255);
          atlas[y][x][3] = 255;
        }
      }
      // Insert glyph info
      rectsByGlyphID.push_back({
        (float) where->bounds.left / ATLAS_SIZE,
        (float) where->bounds.top / ATLAS_SIZE,
        (float) where->bounds.right / ATLAS_SIZE,
        (float) where->bounds.bottom / ATLAS_SIZE
      });
    }
    rek:
    FT_Done_Face(face);
    if (error == 0) {
      stash();
      return;
    }
    throw FTException(error);
  }
}
