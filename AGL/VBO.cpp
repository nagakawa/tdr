#include "VBO.h"

VBO::VBO() {
	glGenBuffers(1, &id);
}

VBO::~VBO() {
}

void VBO::setActive() {
	glBindBuffer(GL_ARRAY_BUFFER, id);
}

void VBO::feedData(GLint size, void * data, GLenum usage) {
	setActive(); // automagically set as active to make life easier
	glBufferData(GL_ARRAY_BUFFER, size, data, usage);
}
