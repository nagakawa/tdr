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
		void feedData(GLint size, void* data, GLenum usage);
		void feedSubdata(GLintptr offset, GLint size, void* data);
		GLuint id;
	};
}
