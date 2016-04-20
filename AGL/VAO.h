#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

namespace agl {
	void resetVAO();
	class VAO {
	public:
		VAO();
		~VAO();
		void setActive();
		GLuint id;
	};
}

