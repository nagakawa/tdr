#pragma once

#include "Shader.h"

// Glory to abusing
// Preprocessor directives!
// (BTW shader.id or shader->id?)
#define SETUNSP(shader, type, uname, value) \
	glGetUniform##type(glGetUniformLocation((shader).id, name), value)

class ShaderProgram {
public:
	ShaderProgram();
	~ShaderProgram();
	void attach(Shader& shader);
	void link();
	void use();
	GLuint getUniformLocation(const GLchar* name);
	GLuint id;
};

