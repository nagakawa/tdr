#include "VBO.h"

using namespace agl;

VBO::VBO() {
	glGenBuffers(1, &id);
}

VBO::~VBO() {
	glDeleteBuffers(1, &id);
}

void VBO::setActive() {
	glBindBuffer(GL_ARRAY_BUFFER, id);
}

void VBO::feedData(GLint size, const void* data, GLenum usage) {
	setActive(); // automagically set as active to make life easier
	glBufferData(GL_ARRAY_BUFFER, size, data, usage);
}

void agl::resetVBO() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
