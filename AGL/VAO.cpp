#include "VAO.h"

using namespace agl;

VAO::VAO() {
	glGenVertexArrays(1, &id);
}

VAO::~VAO() {
	glDeleteVertexArrays(1, &id);
}

void VAO::setActive() {
	glBindVertexArray(id);
}

void agl::resetVAO() {
	glBindVertexArray(0);
}
