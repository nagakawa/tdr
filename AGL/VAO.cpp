#include "VAO.h"

VAO::VAO() {
	glGenVertexArrays(1, &id);
}

VAO::~VAO() {
	glDeleteVertexArrays(1, &id);
}

void VAO::setActive() {
	glBindVertexArray(id);
}
