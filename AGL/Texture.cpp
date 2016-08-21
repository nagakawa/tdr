#include "Texture.h"

#include <SOIL/SOIL.h>

using namespace agl;

const TexInitInfo agl::DEFAULT_TEX_INIT = {GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, true, true, false};
const TexInitInfo agl::TEX_INIT_FOR_FBO = {GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, false, false, false};
const TexInitInfo agl::TEX_INIT_FOR_FBO_MS = {GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, false, false, true};

agl::Texture::Texture(const char* fname, TexInitInfo info) {
	int w, h;
	if (fname == nullptr)
		throw "File name is null!";
	unsigned char* image = SOIL_load_image(fname, &w, &h, 0, SOIL_LOAD_RGBA);
	setTexture(w, h, image, info);
	SOIL_free_image_data(image);
}

agl::Texture::Texture(int w, int h, unsigned char* data, TexInitInfo info) {
	setTexture(w, h, data, info);
}

#include <stdio.h>
void agl::Texture::setTexture(int w, int h, unsigned char* data, TexInitInfo info) {
	width = w;
	height = h;
	ms = info.multisample;
	GLenum mode = ms ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
	if (info.checkForNullData && data == nullptr)
		throw "Image could not be read!";
	glGenTextures(1, &id);
	//printf(u8"テクスチャは追加された。(%d)\n", id);
	glBindTexture(mode, id);
	if (!info.genMipMap) {
		glTexParameteri(mode, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(mode, GL_TEXTURE_MAX_LEVEL, 0);
	}
	if (ms) glTexImage2DMultisample(mode, 4, info.internalFormat, w, h, GL_TRUE);
	else {
		glTexImage2D(mode, 0, info.internalFormat, w, h, 0, info.texFormat, info.pixelType, data);
		glTexParameteri(mode, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(mode, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(mode, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(mode, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	if (info.genMipMap) glGenerateMipmap(mode);
	glBindTexture(mode, 0);
}

void agl::Texture::changeTexture(int w, int h, unsigned char* data, TexInitInfo info) {
	width = w;
	height = h;
	GLenum mode = ms ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
	ms = info.multisample;
	if (info.checkForNullData && data == nullptr)
		throw "Image could not be read!";
		glBindTexture(mode, id);
		if (!info.genMipMap) {
			glTexParameteri(mode, GL_TEXTURE_BASE_LEVEL, 0);
			glTexParameteri(mode, GL_TEXTURE_MAX_LEVEL, 0);
		}
		if (ms) glTexImage2DMultisample(mode, 4, info.internalFormat, w, h, GL_TRUE);
		else glTexImage2D(mode, 0, info.internalFormat, w, h, 0, info.texFormat, info.pixelType, data);
		glTexParameteri(mode, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(mode, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(mode, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(mode, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		if (info.genMipMap) glGenerateMipmap(mode);
		glBindTexture(mode, 0);
}


agl::Texture::Texture(Texture& t) {
	id = t.id;
	width = t.width;
	height = t.height;
}

agl::Texture::Texture() {
	glGenTextures(1, &id);
}

Texture::~Texture() {
	glDeleteTextures(1, &id);
	//printf(u8"テクスチャは削除された。(%d)\n", id);
}

void Texture::bind() {
	glBindTexture(ms ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D, id);
}

void Texture::bindTo(GLint slot) {
	glActiveTexture(GL_TEXTURE0 + slot);
	bind();
}
