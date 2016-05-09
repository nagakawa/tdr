#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

namespace agl {
	class Texture {
	public:
		Texture(const char* fname, GLenum format = GL_RGBA);
		Texture(int w, int h, unsigned char* data, GLenum format = GL_RGBA);
		Texture(Texture& t);
		Texture();
		~Texture();
		void bind();
		void bindTo(GLint slot);
		GLint getWidth() { return width; }
		GLint getHeight() { return height; }
		void changeTexture(int w, int h, unsigned char* data, GLenum texFormat = GL_RGBA);
		GLuint id;
	private:
		GLint width;
		GLint height;
		void setTexture(int w, int h, unsigned char* data, GLenum texFormat = GL_RGBA);
	};
}

