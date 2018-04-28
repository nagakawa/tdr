#pragma once

#include <stdint.h>
#include <vector>

// Pango
extern "C" {
	#include <pango/pango.h>
}

namespace agl {
  struct GlyphInfo {
    uint32_t index;
    int32_t x, y;
  };
}