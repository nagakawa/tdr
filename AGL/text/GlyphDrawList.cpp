#include "text/GlyphDrawList.h"

#include <string.h>

static void freeGItem(void* item, void* /*userdata*/) {
  pango_item_free((PangoItem*) item);
}

namespace agl {
  std::vector<GlyphInfo> layout(
      PangoContext* context,
      PangoAttrList* attrs,
      const char* text
  ) {
    size_t len = strlen(text);
    GList* items = pango_itemize(
      context,
      text,
      0,
      len,
      attrs,
      nullptr);
    PangoGlyphString* glyphs = pango_glyph_string_new();
    struct UD { PangoGlyphString* glyphs; const char* text; size_t len; };
    UD myud = { glyphs, text, len };
    g_list_foreach(items, [](void* it, void* userdata) {
      PangoItem* item = (PangoItem*) it;
      UD* ud = (UD*) userdata;
      pango_shape_full(
        ud->text + item->offset,
        item->length,
        ud->text,
        ud->len,
        &(item->analysis),
        ud->glyphs
      );
    }, &myud);
    std::vector<GlyphInfo> res(glyphs->num_glyphs);
    for (size_t i = 0; i < glyphs->num_glyphs; ++i) {
      PangoGlyphInfo& g = glyphs->glyphs[i];
      GlyphInfo& out = res[i];
      out.index = g.glyph;
      out.x = g.geometry.x_offset;
      out.y = g.geometry.y_offset;
    }
    // Cleanup
    pango_glyph_string_free(glyphs);
    g_list_foreach(items, freeGItem, nullptr);
    g_list_free(items);
    return res;
  }
}