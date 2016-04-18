#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

namespace agl {
	class VAO {
	public:
		VAO();
		~VAO();
		void setActive();
		GLuint id;
	};
}

