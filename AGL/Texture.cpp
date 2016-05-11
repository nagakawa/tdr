#include "Texture.h"

#include <SOIL/SOIL.h>

using namespace agl;

agl::Texture::Texture(const char* fname, GLenum format) {
	int w, h;
	if (fname == nullptr)
		throw "File name is null!";
	unsigned char* image = SOIL_load_image(fname, &w, &h, 0, SOIL_LOAD_RGBA);
	setTexture(w, h, image, format);
	SOIL_free_image_data(image);
}

agl::Texture::Texture(int w, int h, unsigned char* data, GLenum format) {
	setTexture(w, h, data, format);
}

void agl::Texture::setTexture(int w, int h, unsigned char* data, GLenum texFormat) {
	width = w;
	height = h;
	if (data == nullptr)
		throw "Image could not be read!";
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, texFormat, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void agl::Texture::changeTexture(int w, int h, unsigned char * data, GLenum texFormat) {
	width = w;
	height = h;
	if (data == nullptr)
		throw "Image could not be read!";
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, texFormat, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
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
}

void Texture::bind() {
	glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::bindTo(GLint slot) {
	glActiveTexture(GL_TEXTURE0 + slot);
	bind();
}
