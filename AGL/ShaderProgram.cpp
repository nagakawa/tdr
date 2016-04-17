#include "ShaderProgram.h"

ShaderProgram::ShaderProgram() {
	id = glCreateProgram();
}

ShaderProgram::~ShaderProgram() {
	glDeleteProgram(id);
}

// Hmm, Shader shader or Shader& shader? I wonder...
void ShaderProgram::attach(Shader& shader) {
	glAttachShader(id, shader.id);
}

void ShaderProgram::link() {
	glLinkProgram(id);
	GLint success;
	GLchar infoLog[512];
	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(id, 512, NULL, infoLog);
		throw infoLog;
	}
}

void ShaderProgram::use() {
	glUseProgram(id);
}

GLuint ShaderProgram::getUniformLocation(const GLchar* name) {
	return glGetUniformLocation(id, name);
}
