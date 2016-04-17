#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

class Texture {
public:
	Texture(const char* fname);
	~Texture();
	void bind();
	void bindTo(GLint slot);
	GLuint id;
};
