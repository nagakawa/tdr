#include "Texture.h"

#include <SOIL/SOIL.h>

using namespace agl;

const TexInitInfo agl::DEFAULT_TEX_INIT = {GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, true, true, false};
const TexInitInfo agl::TEX_INIT_FOR_FBO = {GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, false, false, false};
const TexInitInfo agl::TEX_INIT_FOR_FBO_MS = {GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, false, false, true};
#include <iostream>
agl::Texture::Texture(const char* fname, const TexInitInfo& info) {
	int w, h;
	if (fname == nullptr)
		throw "File name is null!";
	unsigned char* image = SOIL_load_image(fname, &w, &h, nullptr, SOIL_LOAD_RGBA);
	setTexture(w, h, image, info);
	SOIL_free_image_data(image);
	//std::cerr << fname << " ~> " << id << '\n';
}

agl::Texture::Texture(const uint8_t* buffer, int bufferLength, const TexInitInfo& info) {
	int w, h;
	unsigned char* image = SOIL_load_image_from_memory(buffer, bufferLength, &w, &h, nullptr, SOIL_LOAD_RGBA);
	setTexture(w, h, image, info);
	SOIL_free_image_data(image);
	//std::cerr << fname << " ~> " << id << '\n';
}

agl::Texture::Texture(int w, int h, unsigned char* data, const TexInitInfo& info) {
	setTexture(w, h, data, info);
}

#include <debug.h>
void agl::Texture::setTexture(int w, int h, unsigned char* data, const TexInitInfo& info) {
	changeTexture(w, h, data, info, true);
}

void agl::Texture::changeTexture(int w, int h, unsigned char* data, const TexInitInfo& info, bool genNew) {
	width = w;
	height = h;
	ms = info.multisample;
	GLenum mode = ms ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
	if (info.checkForNullData && data == nullptr)
		throw "Image could not be read!";
	if (genNew) glGenTextures(1, &id);
	//std::cerr << id << " " << ms << "\n";
	glBindTexture(mode, id);
	//std::cerr << "done.\n";
	if (!info.genMipMap) {
		glTexParameteri(mode, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(mode, GL_TEXTURE_MAX_LEVEL, 0);
	}
	if (ms) {
		glTexImage2DMultisample(mode, 4, info.internalFormat, w, h, GL_TRUE);
	}
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


/*agl::Texture::Texture(const Texture& t) {
	id = t.id;
	width = t.width;
	height = t.height;
}*/

agl::Texture::Texture(Texture&& t) {
	id = t.id;
	width = t.width;
	height = t.height;
	ms = t.ms;
	t.id = 0;
}

Texture& agl::Texture::operator=(Texture&& t) {
	id = t.id;
	width = t.width;
	height = t.height;
	ms = t.ms;
	t.id = 0;
	return *this;
}

agl::Texture::Texture() {
	glGenTextures(1, &id);
}

Texture::~Texture() {
	glDeleteTextures(1, &id);
	//printf(u8"テクスチャは削除された。(%d)\n", id);
}

void Texture::bind() const {
	glBindTexture(ms ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D, id);
}

void Texture::bindTo(GLint slot) const {
	glActiveTexture(GL_TEXTURE0 + slot);
	bind();
}

agl::Texture3::Texture3(const char* fname, int divide, const TexInitInfo& info) {
	int w, h;
	if (fname == nullptr)
		throw "File name is null!";
	unsigned char* image = SOIL_load_image(fname, &w, &h, nullptr, SOIL_LOAD_RGBA);
	setTexture(w, divide, h / divide, image, info);
	SOIL_free_image_data(image);
	//std::cerr << fname << " ~> " << id << '\n';
}

agl::Texture3::Texture3(const uint8_t* buffer, int divide, int bufferLength, const TexInitInfo& info) {
	int w, h;
	unsigned char* image = SOIL_load_image_from_memory(buffer, bufferLength, &w, &h, nullptr, SOIL_LOAD_RGBA);
	setTexture(w, divide, h / divide, image, info);
	SOIL_free_image_data(image);
	//std::cerr << fname << " ~> " << id << '\n';
}

agl::Texture3::Texture3(int w, int h, int d, unsigned char* data, const TexInitInfo& info) {
	setTexture(w, h, d, data, info);
}

void agl::Texture3::setTexture(int w, int h, int d, unsigned char* data, const TexInitInfo& info) {
	changeTexture(w, h, d, data, info, true);
}

void agl::Texture3::changeTexture(int w, int h, int d, unsigned char* data, const TexInitInfo& info, bool genNew) {
	width = w;
	height = h;
	depth = d;
	GLenum mode = GL_TEXTURE_3D;
	if (info.checkForNullData && data == nullptr)
		throw "Image could not be read!";
	if (genNew) glGenTextures(1, &id);
	//std::cerr << id << " " << ms << "\n";
	glBindTexture(mode, id);
	//std::cerr << "done.\n";
	if (!info.genMipMap) {
		glTexParameteri(mode, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(mode, GL_TEXTURE_MAX_LEVEL, 0);
	}
	else {
		glTexImage3D(mode, 0, info.internalFormat, w, h, d, 0, info.texFormat, info.pixelType, data);
		glTexParameteri(mode, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(mode, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(mode, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(mode, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	if (info.genMipMap) glGenerateMipmap(mode);
	glBindTexture(mode, 0);
}


/*agl::Texture::Texture(const Texture& t) {
	id = t.id;
	width = t.width;
	height = t.height;
}*/

agl::Texture3::Texture3(Texture3&& t) {
	id = t.id;
	width = t.width;
	height = t.height;
	depth = t.depth;
	t.id = 0;
}

agl::Texture3::Texture3() {
	glGenTextures(1, &id);
}

Texture3::~Texture3() {
	glDeleteTextures(1, &id);
	//printf(u8"テクスチャは削除された。(%d)\n", id);
}

void Texture3::bind() {
	glBindTexture(GL_TEXTURE_3D, id);
}

void Texture3::bindTo(GLint slot) {
	glActiveTexture(GL_TEXTURE0 + slot);
	bind();
}
