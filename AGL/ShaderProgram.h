#pragma once

#include "Shader.h"

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

