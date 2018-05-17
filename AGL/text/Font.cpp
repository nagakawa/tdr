#include "Font.h"

#include <string.h>
#include <time.h>
#include <iostream>
#include <memory>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_OUTLINE_H
#include <hb.h>
#include <hb-ft.h>

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
    UIRect16 bounds;
    size_t glyph = -1U;
    Node* insert(uint16_t w, uint16_t h, size_t gid) {
      if (child[0] != nullptr) {
        Node* nn = child[0]->insert(w, h, gid);
        if (nn != nullptr) return nn;
        return child[1]->insert(w, h, gid);
      }
      if (glyph != -1U) return nullptr;
      if (w > bounds.right - bounds.left ||
          h > bounds.bottom - bounds.top) {
        return nullptr;
      }
      if (w == bounds.right - bounds.left &&
          h == bounds.bottom - bounds.top) {
        glyph = gid;
        return this;
      }
      child[0] = std::make_unique<Node>();
      child[1] = std::make_unique<Node>();
      uint16_t dw = bounds.right - bounds.left - w;
      uint16_t dh = bounds.bottom - bounds.top - h;
      if (dw > dh) {
        child[0]->bounds = {
          bounds.left, bounds.top,
          (uint16_t) (bounds.left + w), bounds.bottom
        };
        child[1]->bounds = {
          (uint16_t) (bounds.left + w), bounds.top,
          bounds.right, bounds.bottom
        };
      } else {
        child[0]->bounds = {
          bounds.left, bounds.top,
          bounds.right, uint16_t(bounds.top + h)
        };
        child[1]->bounds = {
          bounds.left, uint16_t(bounds.top + h),
          bounds.right, bounds.bottom
        };
      }
      return child[0]->insert(w, h, gid);
    }
  };

  TexInitInfo atlasInfo = {
    /* internalFormat = */ GL_RGBA,
    /* texFormat = */ GL_RGBA,
    /* pixelType = */ GL_UNSIGNED_BYTE,
    /* checkForNullData = */ false,
    /* genMipMap = */ false,
    /* multisample = */ false
  };

  Font::Font(FT_Library ftl, const char* filename, size_t s)
      : size(s) {
    std::cerr << "Creating font from " << filename << "\n";
    clock_t cl = clock();
    FT_Error error = FT_New_Face(ftl, filename, 0, &face);
    if (error != 0) throw FTException(error);
    error = FT_Set_Char_Size(face, size * 64, size * 64, 0, 0);
    if (error != 0) throw FTException(error);
    uint32_t* atlas = new uint32_t[ATLAS_SIZE * ATLAS_SIZE];
    Node n;
    n.bounds = { 0, 0, ATLAS_SIZE, ATLAS_SIZE };
    size_t texid = 0;
    auto stash = [this, atlas, &texid]() {
      texs.emplace_back(ATLAS_SIZE, ATLAS_SIZE, (uint8_t*) atlas, atlasInfo);
      ++texid;
    };
    // Texture packing algorithm from 
    // http://blackpawn.com/texts/lightmaps/default.html
    for (unsigned i = 0; i < (size_t) face->num_glyphs; ++i) {
      // Load the glyph
      error = FT_Load_Glyph(face, i, FT_LOAD_NO_BITMAP);
      if (error != 0) goto rek;
      msdfgen::Shape output;
      output.contours.clear();
      output.inverseYAxis = false;
      FT_Outline* outline = &(face->glyph->outline);
      // Deduce outline direction
      bool reversed =
        FT_Outline_Get_Orientation(outline) == FT_ORIENTATION_POSTSCRIPT;
      FtContext ctx;
      ctx.shape = &output;
      error = FT_Outline_Decompose(
        outline,
        &myOutlineFuncs,
        &ctx
      );
      if (error != 0) goto rek;
      // Generate the MSDF
      output.normalize();
      msdfgen::edgeColoringSimple(output, 3.0);
      size_t m = margin();
      FT_BBox bb;
      FT_Outline_Get_CBox(outline, &bb);
      uint32_t gw = (bb.xMax - bb.xMin + 32) / 64;
      uint32_t gh = (bb.yMax - bb.yMin + 32) / 64;
      msdfgen::Bitmap<msdfgen::FloatRGB> bm(
        gw + 2 * m,
        gh + 2 * m
      );
      uint32_t w = bm.width(), h = bm.height();
      msdfgen::generateMSDF(
        bm,
        output,
        m,
        1.0,
        msdfgen::Vector2(m - bb.xMin / 64.0, m - bb.yMin / 64.0)
      );
      if (reversed) {
        for (uint32_t y = 0; y < h; ++y) {
          for (uint32_t x = 0; x < w; ++x) {
            auto& p = bm(x, y);
            p.r = 1 - p.r;
            p.g = 1 - p.g;
            p.b = 1 - p.b;
          }
        }
      }
      // Copy the MSDF into the texture
      Node* where = n.insert(w + 1, h + 1, i);
      if (where == nullptr) {
        stash();
        n = Node();
        n.bounds = { 0, 0, ATLAS_SIZE, ATLAS_SIZE };
        where = n.insert(w + 1, h + 1, i);
        if (where == nullptr) throw "really?";
      }
      const auto& bounds = where->bounds;
      for (uint32_t y = bounds.top; y < bounds.bottom - 1; ++y) {
        int ty = y - bounds.top;
        msdfgen::FloatRGB* p = &(bm(0, ty));
        for (uint32_t x = bounds.left; x < bounds.right - 1; ++x) {
          int tx = x - bounds.left;
          auto col = p[tx];
          uint32_t pixel =
            (uint32_t) (std::min(1.0f, std::max(0.0f, col.r)) * 255) |
            ((uint32_t) (std::min(1.0f, std::max(0.0f, col.g)) * 255) << 8) |
            ((uint32_t) (std::min(1.0f, std::max(0.0f, col.b)) * 255) << 16) |
            0xFF000000;
          atlas[ATLAS_SIZE * y + x] = pixel;
        }
      }
      // Insert glyph info
      rectsByGlyphID.push_back({
        texid,
        {
          bounds.left, bounds.top,
          bounds.right, bounds.bottom
        },
        (int32_t) gw,
        (int32_t) gh,
        (int32_t) bb.xMin,
        (int32_t) bb.yMin,
      });
    }
    facehb = hb_ft_font_create(face, nullptr);
    rek:
    if (error == 0) {
      stash();
      delete[] atlas;
      std::cerr << texs.size() << " page(s)\n";
      std::cerr << (double) (clock() - cl) / CLOCKS_PER_SEC <<
        " seconds elapsed\n";
      return;
    }
    delete[] atlas;
    throw FTException(error);
  }
  Font::~Font() {
    hb_font_destroy(facehb);
    FT_Done_Face(face);
  }
}
