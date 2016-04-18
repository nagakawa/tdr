#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

namespace agl {
	class EBO {
	public:
		EBO();
		~EBO();
		void setActive();
		void feedData(GLint size, void* data, GLenum usage);
		GLuint id;
	};
}
