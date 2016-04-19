#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

namespace agl {
	class Texture {
	public:
		Texture(const char* fname);
		Texture(Texture& t);
		~Texture();
		void bind();
		void bindTo(GLint slot);
		GLint getWidth() { return width; }
		GLint getHeight() { return height; }
		GLuint id;
	private:
		GLint width;
		GLint height;
	};
}

