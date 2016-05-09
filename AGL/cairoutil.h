#pragma once

#include <pango/pangocairo.h>
#include <stdio.h>
#include <string.h>

#include "Texture.h"

namespace agl {
	cairo_t* createCairoContext(
		int width, int height, int channels,
		cairo_surface_t*& surface,
		unsigned char*& buffer,
		bool updated
		);
	cairo_t* createLayoutContext();
	void getTextSize(PangoLayout& layout, unsigned int& width, unsigned int& height);
	void renderText(unsigned char*& buffer, const char* text, const char* font, unsigned int& width, unsigned int& height, unsigned int margin, double fontSize, Texture& t, bool rich);
}
