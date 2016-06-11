#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

namespace agl {
	struct TexInitInfo {
		GLenum internalFormat;
		GLenum texFormat;
		GLenum pixelType;
		bool checkForNullData;
		bool genMipMap;
		bool multisample;
	};
	#define DEFAULT_TEX_INIT ((TexInitInfo) {GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, true, true, false})
	#define TEX_INIT_FOR_FBO ((TexInitInfo) {GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, false, false, false})
	#define TEX_INIT_FOR_FBO_MS ((TexInitInfo) {GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, false, false, true})
	class Texture {
	public:
		Texture(const char* fname, TexInitInfo info = DEFAULT_TEX_INIT);
		Texture(int w, int h, unsigned char* data, TexInitInfo info = DEFAULT_TEX_INIT);
		Texture(Texture& t);
		Texture();
		~Texture();
		void bind();
		void bindTo(GLint slot);
		GLint getWidth() { return width; }
		GLint getHeight() { return height; }
		void changeTexture(int w, int h, unsigned char* data, TexInitInfo info = DEFAULT_TEX_INIT);
		GLuint id;
	private:
		GLint width;
		GLint height;
		bool ms;
		void setTexture(int w, int h, unsigned char* data, TexInitInfo info = DEFAULT_TEX_INIT);
	};
}
