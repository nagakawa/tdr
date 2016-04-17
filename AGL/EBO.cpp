#include "EBO.h"

EBO::EBO() {
	glGenBuffers(1, &id);
}

EBO::~EBO() {
	glDeleteBuffers(1, &id);
}

void EBO::setActive() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}

void EBO::feedData(GLint size, void * data, GLenum usage) {
	setActive(); // automagically set as active to make life easier
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, usage);
}
