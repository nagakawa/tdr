#pragma once

#include <stdint.h>
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
		// Using int instead of size_t for the buffer size here, because SOIL uses
		// that type in SOIL_load_image_from_memory. Uugghh.
		Texture(const uint8_t* buffer, int bufferLength, const TexInitInfo& info = DEFAULT_TEX_INIT);
		Texture(int w, int h, unsigned char* data, const TexInitInfo& info = DEFAULT_TEX_INIT);
		// Texture(const Texture& t);
		Texture(Texture&& t);
		Texture& operator=(Texture&& t);
		Texture();
		~Texture();
		void bind() const;
		void bindTo(GLint slot) const;
		GLint getWidth() const { return width; }
		GLint getHeight() const { return height; }
		void changeTexture(int w, int h, unsigned char* data, const TexInitInfo& info = DEFAULT_TEX_INIT, bool genNew = false);
		GLuint id;
	private:
		GLint width;
		GLint height;
		bool ms = false;
		void setTexture(int w, int h, unsigned char* data, const TexInitInfo& info = DEFAULT_TEX_INIT);
	};
	class Texture3 {
	public:
		Texture3(const char* fname, int divide, const TexInitInfo& info = DEFAULT_TEX_INIT);
		// Using int instead of size_t for the buffer size here, because SOIL uses
		// that type in SOIL_load_image_from_memory. Uugghh.
		Texture3(const uint8_t* buffer, int divide, int bufferLength, const TexInitInfo& info = DEFAULT_TEX_INIT);
		Texture3(int w, int h, int d, unsigned char* data, const TexInitInfo& info = DEFAULT_TEX_INIT);
		// Texture(const Texture& t);
		Texture3(Texture3&& t);
		Texture3();
		~Texture3();
		void bind();
		void bindTo(GLint slot);
		GLint getWidth() const { return width; }
		GLint getHeight() const { return height; }
		GLint getDepth() const { return depth; }
		void changeTexture(int w, int h, int d, unsigned char* data, const TexInitInfo& info = DEFAULT_TEX_INIT, bool genNew = false);
		GLuint id;
	private:
		GLint width;
		GLint height;
		GLint depth;
		bool ms = false;
		void setTexture(int w, int h, int d, unsigned char* data, const TexInitInfo& info = DEFAULT_TEX_INIT);
	};
}
