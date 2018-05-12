#include "text/GlyphDrawList.h"

#include <string.h>
#include <iostream>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_OUTLINE_H
#include <hb.h>

#define U_CHARSET_IS_UTF8 1
#include <unicode/brkiter.h>
#include <unicode/ubidi.h>

#include "Font.h"

// Shared break iterator to avoid constructing redundant objects
static thread_local icu::BreakIterator* bi = nullptr;
static thread_local UBiDi* para = nullptr;
static thread_local UBiDi* line = nullptr;
static thread_local hb_buffer_t* hbb = nullptr;

namespace agl {
  std::vector<GlyphInfo> layoutText(
      const char* utf8,
      const LayoutInfo& layout) {
    std::cerr << "Laying out text: " << utf8 << "\n";
    size_t fontSize = layout.f->getSize();
    UErrorCode status = U_ZERO_ERROR;
    // Initialise bidi stuffs
    if (para == nullptr) para = ubidi_open();
    if (para == nullptr) abort(); // not redundant
    if (line == nullptr) line = ubidi_open();
    if (line == nullptr) abort(); // not redundant
    // Initialise HarfBuzz buffer
    if (hbb == nullptr) hbb = hb_buffer_create();
    // Create a UnicodeString for ICU calls
    icu::UnicodeString s(utf8);
    int32_t len = s.length();
    // Find the line break boundaries
    if (bi == nullptr) {
      bi = icu::BreakIterator::createLineInstance(icu::Locale(), status);
      if (U_FAILURE(status)) abort();
    }
    bi->setText(s);
    // Take each run of characters between break boundaries
    // and split them into bidi runs and collect them
    std::vector<Run> runs;
    ubidi_setPara(
      para,
      s.getBuffer(),
      len,
      UBIDI_DEFAULT_LTR,
      nullptr, &status
    );
    if (U_FAILURE(status)) abort();
    for (int32_t p = bi->first();
         p != icu::BreakIterator::DONE; ) {
      int32_t pn = bi->next();
      int32_t limit = (pn == icu::BreakIterator::DONE) ? len : pn;
      if (p == limit) {
        p = pn;
        continue;
      }
      // Do bidi stuffs
      ubidi_setLine(para, p, limit, line, &status);
      if (U_FAILURE(status)) abort();
      int32_t nRuns = ubidi_countRuns(line, &status);
      if (U_FAILURE(status)) abort();
      for (int32_t rIndex = 0; rIndex < nRuns; ++rIndex) {
        runs.emplace_back();
        Run& run = runs.back();
        int32_t start, len;
        ubidi_getVisualRun(line, rIndex, &start, &len);
        run.start = start;
        run.len = len;
        // Layout using HarfBuzz
        hb_buffer_clear_contents(hbb);
        hb_buffer_add_utf16(hbb, s.getBuffer() + p + start, len, 0, len);
        hb_buffer_guess_segment_properties(hbb);
        hb_font_t* font = layout.f->getHBFont();
        hb_shape(font, hbb, nullptr, 0);
        unsigned nGlyphs;
        hb_glyph_info_t* glyphInfo =
          hb_buffer_get_glyph_infos(hbb, &nGlyphs);
        hb_glyph_position_t* glyphPos =
          hb_buffer_get_glyph_positions(hbb, &nGlyphs);
        run.glyphs.resize(nGlyphs);
        int32_t cursorX = 0, cursorY = 0;
        int32_t minX = INT32_MAX, minY = INT32_MAX;
        int32_t maxX = INT32_MIN, maxY = INT32_MIN;
        for (unsigned i = 0; i < nGlyphs; ++i) {
          // These fields are in font units times 64.
          // Divide by the font's size and multiply by the requested size
          // to get the size we want.
          int32_t x = cursorX + glyphPos[i].x_offset * layout.fontSize / fontSize;
          int32_t y = cursorY + glyphPos[i].y_offset * layout.fontSize / fontSize;
          Font::GlyphInfo& gi = layout.f->getInfo(glyphInfo[i].codepoint);
          int32_t w = (int32_t) (gi.w * layout.fontSize / fontSize);
          int32_t h = (int32_t) (gi.h * layout.fontSize / fontSize);
          run.glyphs[i] = {
            /* .index = */ glyphInfo[i].codepoint,
            /* .x = */ x,
            /* .y = */ y,
            /* .w = */ w,
            /* .h = */ h
          };
          minX = std::min(minX, x); minY = std::min(minY, y);
          maxX = std::max(maxX, x + w * 64); maxY = std::max(maxY, y + h * 64);
          cursorX += glyphPos[i].x_advance * layout.fontSize / fontSize;
          cursorY += glyphPos[i].y_advance * layout.fontSize / fontSize;
        }
        minX = std::min(minX, cursorX); minY = std::min(minY, cursorY);
        maxX = std::max(maxX, cursorX); maxY = std::max(maxY, cursorY);
        run.width = maxX - minX + layout.margin;
        run.height = maxY - minY + layout.margin;
        run.offx = -layout.margin - minX;
        run.offy = -layout.margin - minY;
      }
      // done
      p = pn;
    }
    // How many runs fit on a line?
    std::vector<GlyphInfo> out;
    size_t cursorY = 0;
    size_t cursorX = 0;
    size_t netWidth = layout.maxWidth - layout.margin;
    size_t start = 0, i;
    auto genLine = [&runs, &out, &cursorY, &cursorX, &start, &i, &layout]() {
      // Get max height of runs
      int32_t maxh = 0;
      for (size_t j = start; j < i; ++j) {
        maxh = std::max(maxh, runs[j].height);
      }
      int32_t myX = 0;
      // Copy runs
      for (size_t j = start; j < i; ++j) {
        const Run& run = runs[j];
        for (const GlyphInfo& gi : run.glyphs) {
          int32_t newX = gi.x + myX - run.offx;
          int32_t newY = gi.y + cursorY - run.offy;
          GlyphInfo newGI = {gi.index, newX, newY, gi.w, gi.h};
          out.push_back(newGI);
        }
        myX += run.width;
      }
      cursorX = 0;
      cursorY += maxh + layout.lineSkip * 64;
    };
    for (i = 0; i < runs.size(); ++i) {
      const Run& run = runs[i];
      // Does this run fit on the line?
      // If this will be the only run on the line, then let it overflow.
      if (run.width + cursorX > netWidth * 64 && cursorX != 0) {
        genLine();
        start = i;
      }
      cursorX += run.width;
    }
    genLine();
    return out;
  }
}