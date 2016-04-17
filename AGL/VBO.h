#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

class VBO {
public:
	VBO();
	~VBO();
	void setActive();
	void feedData(GLint size, void* data, GLenum usage);
	GLuint id;
};
