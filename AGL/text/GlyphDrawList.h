#pragma once

#include <stdint.h>
#include <vector>

namespace agl {
  struct GlyphInfo {
    uint32_t index;
    int32_t x, y; // 6 fractional bits
  };
  struct Run {
    std::vector<GlyphInfo> glyphs;
    int32_t start, len;
    int32_t width, height;
    // Offset x and y to convert from coordinates of the glyphs to those
    // relative to the top-left corner.
    int32_t offx, offy;
  };
  class Font;
  struct LayoutInfo {
    Font* f;
    size_t maxWidth;
    size_t fontSize;
    size_t margin;
    size_t lineSkip;
  };
  std::vector<GlyphInfo> layoutText(
      const char* utf8,
      const LayoutInfo& layout);
}