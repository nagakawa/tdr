#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

namespace agl {
	struct Rect {
		GLfloat left, top, right, bottom;
	};
	const GLfloat rectangleVertices[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f
	};
}
