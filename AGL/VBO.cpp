#include "VBO.h"

using namespace agl;

agl::VBO::VBO() {
	glGenBuffers(1, &id);
}

agl::VBO::~VBO() {
	glDeleteBuffers(1, &id);
}

void agl::VBO::setActive() {
	glBindBuffer(GL_ARRAY_BUFFER, id);
}

void agl::VBO::feedData(GLint size, void * data, GLenum usage) {
	setActive(); // automagically set as active to make life easier
	glBufferData(GL_ARRAY_BUFFER, size, data, usage);
}

void agl::VBO::feedSubdata(
		GLintptr offset,GLint size, void * data) {
	setActive(); // automagically set as active to make life easier
	glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
}

void agl::resetVBO() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
