#pragma once

#include <GL/glew.h>

class Texture {
public:
	Texture(const char* fname);
	~Texture();
	void bind();
	GLuint id;
};

