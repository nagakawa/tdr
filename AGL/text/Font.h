#pragma once

#include <stdexcept>
#include <vector>

#include "Texture.h"
#include "rect.h"

struct FT_LibraryRec_;

namespace agl {
  constexpr size_t ATLAS_SIZE = 4096;
  const char* ftErrorToString(int code);
  class FTException : std::exception {
  public:
    FTException(int code) : code(code) {}
    const char* what() const noexcept {
      return ftErrorToString(code);
    }
  private:
    int code;
  };
  class Font {
  public:
    Font(FT_LibraryRec_* ftl, const char* filename, size_t size);
  private:
    std::vector<Texture> texs;
    std::vector<Rect> rectsByGlyphID;
  };
}
