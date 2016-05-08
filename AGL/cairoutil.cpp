#include "cairoutil.h"

using namespace agl;

// Functions from here, C++-ified
// https://dthompson.us/font-rendering-in-opengl-with-pango-and-cairo.html

cairo_t* agl::createCairoContext(
	int width, int height, int channels,
	cairo_surface_t*& surface,
	unsigned char*& buffer
	) {
	buffer = new unsigned char[channels * width * height];
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

void agl::renderText(const char* text, const char* font, unsigned int& width, unsigned int& height, unsigned int margin, double fontSize, Texture& t) {
	if (margin <= 2) throw "Margin is too narrow! It must be greater than 2.";
	cairo_t* layoutContext = createLayoutContext();
	PangoLayout* layout = pango_cairo_create_layout(layoutContext);
	pango_layout_set_text(layout, text, -1);
	pango_layout_set_width(layout, margin * PANGO_SCALE);
	pango_layout_set_wrap(layout, PANGO_WRAP_WORD);
	PangoFontDescription* desc = pango_font_description_from_string(font);
	pango_font_description_set_absolute_size(desc, fontSize * PANGO_SCALE * 20);
	pango_layout_set_font_description(layout, desc);
	pango_font_description_free(desc);
	getTextSize(*layout, width, height);
	cairo_surface_t* surface;
	unsigned char* surfaceData;
	cairo_t* renderContext = createCairoContext(width, height, 4, surface, surfaceData);
	/*PangoContext* pangoContext = pango_cairo_create_context(renderContext);
	cairo_font_options_t* fontOptions = cairo_font_options_create();
	cairo_font_options_set_antialias(fontOptions, CAIRO_ANTIALIAS_BEST);
	pango_cairo_context_set_font_options(pangoContext, fontOptions);*/
	cairo_set_source_rgba(renderContext, 1, 1, 1, 1);
	pango_cairo_show_layout(renderContext, layout);
	t.setTexture(width, height, surfaceData, GL_BGRA);
	free(surfaceData);
	g_object_unref(layout);
	cairo_destroy(layoutContext);
	cairo_destroy(renderContext);
	cairo_surface_destroy(surface);
}
