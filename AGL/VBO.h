#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

namespace agl {
	void resetVBO();
	class VBO {
	public:
		VBO();
		~VBO();
		void setActive();
		void feedData(GLint size, const void* data, GLenum usage);
		GLuint id;
	};
}
