#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#ifdef _WIN32
#warning "Do not build on Windows. Build on another OS instead."
#endif

namespace agl {
	template<typename T>
	struct TRect {
		T left, top, right, bottom;
	};
	using Rect = TRect<GLfloat>;
	using UIRect16 = TRect<uint16_t>;
	using IRect16 = TRect<int16_t>;
	const GLfloat rectangleVertices[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f
	};
}
