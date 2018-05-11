#pragma once

#include <stdexcept>
#include <vector>

#include "Texture.h"
#include "rect.h"

struct FT_LibraryRec_;
struct FT_FaceRec_;
struct hb_font_t;

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
    struct GlyphInfo {
      size_t texid;
      Rect box;
      int32_t w, h;
    };
    Font(FT_LibraryRec_* ftl, const char* filename, size_t size);
    ~Font();
    FT_FaceRec_* getFont() { return face; }
    hb_font_t* getHBFont() { return facehb; }
    size_t getSize() { return size; }
    GlyphInfo& getInfo(uint32_t glyphID) {
      return rectsByGlyphID[glyphID];
    }
  private:
    std::vector<Texture> texs;
    std::vector<GlyphInfo> rectsByGlyphID;
    FT_FaceRec_* face;
    hb_font_t* facehb;
    size_t size;
    friend class XText;
  };
}
