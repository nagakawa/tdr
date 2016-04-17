#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

class VAO {
public:
	VAO();
	~VAO();
	void setActive();
	GLuint id;
};

