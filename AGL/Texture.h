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
	extern const TexInitInfo DEFAULT_TEX_INIT;
	extern const TexInitInfo TEX_INIT_FOR_FBO;
	extern const TexInitInfo TEX_INIT_FOR_FBO_MS;
	class Texture {
	public:
		Texture(const char* fname, const TexInitInfo& info = DEFAULT_TEX_INIT);
		Texture(int w, int h, unsigned char* data, const TexInitInfo& info = DEFAULT_TEX_INIT);
		Texture(const Texture& t);
		Texture();
		~Texture();
		void bind();
		void bindTo(GLint slot);
		GLint getWidth() const { return width; }
		GLint getHeight() const { return height; }
		void changeTexture(int w, int h, unsigned char* data, const TexInitInfo& info = DEFAULT_TEX_INIT, bool genNew = false);
		GLuint id;
	private:
		GLint width;
		GLint height;
		bool ms;
		void setTexture(int w, int h, unsigned char* data, const TexInitInfo& info = DEFAULT_TEX_INIT);
	};
}
