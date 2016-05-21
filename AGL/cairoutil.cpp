#include "cairoutil.h"

using namespace agl;

// Functions from here, C++-ified
// https://dthompson.us/font-rendering-in-opengl-with-pango-and-cairo.html

cairo_t* agl::createCairoContext(
	int width, int height, int channels,
	cairo_surface_t*& surface,
	unsigned char*& buffer,
	bool updated
	) {
	if (!updated && buffer != nullptr) {
		memset(buffer, 0, width * height * channels);
	} else {
		free(buffer);
		buffer = (unsigned char*) calloc(channels * width * height, sizeof(unsigned char));
	}
	surface = cairo_image_surface_create_for_data(buffer, CAIRO_FORMAT_ARGB32, width, height, channels * width);
	cairo_t* context = cairo_create(surface);
	return context;
}

cairo_t* agl::createLayoutContext() {
	cairo_surface_t* tempSurface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 0, 0);
	cairo_t* context = cairo_create(tempSurface);
	cairo_surface_destroy(tempSurface);
	return context;
}

void agl::getTextSize(PangoLayout& layout, unsigned int& width, unsigned int& height) {
	pango_layout_get_size(&layout, (int*) &width, (int*) &height);
	width /= PANGO_SCALE;
	height /= PANGO_SCALE;
}

void agl::renderText(unsigned char*& buffer, const char* text, const char* font, unsigned int& width, unsigned int& height, unsigned int margin, double fontSize, Texture& t, bool rich) {
	cairo_t* layoutContext = createLayoutContext();
	PangoLayout* layout = pango_cairo_create_layout(layoutContext);
	if (rich) {
		PangoAttrList* al = nullptr;
		char* pt = nullptr;
		GError* err = nullptr;
		gboolean success = pango_parse_markup(text, -1, 0, &al, &pt, nullptr, &err);
		if (!success) {
			throw err->message;
		}
		pango_layout_set_text(layout, pt, -1);
		pango_layout_set_attributes(layout, al);
		pango_attr_list_unref(al);
		g_free(pt);
		if (err != nullptr) g_error_free(err);
	} else {
		pango_layout_set_text(layout, text, -1);
	}
	pango_layout_set_width(layout, margin * PANGO_SCALE);
	pango_layout_set_wrap(layout, PANGO_WRAP_WORD);
	PangoFontDescription* desc = pango_font_description_from_string(font);
	pango_font_description_set_absolute_size(desc, fontSize * PANGO_SCALE * 20);
	pango_layout_set_font_description(layout, desc);
	pango_font_description_free(desc);
	unsigned int oldWidth = width, oldHeight = height;
	getTextSize(*layout, width, height);
	bool updated = oldWidth * oldHeight != width * height;
	cairo_surface_t* surface;
	cairo_t* renderContext = createCairoContext(width, height, 4, surface, buffer, updated);
	/*PangoContext* pangoContext = pango_cairo_create_context(renderContext);
	cairo_font_options_t* fontOptions = cairo_font_options_create();
	cairo_font_options_set_antialias(fontOptions, CAIRO_ANTIALIAS_BEST);
	pango_cairo_context_set_font_options(pangoContext, fontOptions);*/
	cairo_set_source_rgba(renderContext, 1, 1, 1, 1);
	pango_cairo_show_layout(renderContext, layout);
	t.changeTexture(width, height, buffer, {GL_RGBA, GL_BGRA, GL_UNSIGNED_BYTE, true});
	g_object_unref(layout);
	cairo_destroy(layoutContext);
	cairo_destroy(renderContext);
	cairo_surface_destroy(surface);
}
