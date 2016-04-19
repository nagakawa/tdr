#pragma once

#include "Shader.h"

// Glory to abusing
// Preprocessor directives!
// (BTW shader.id or shader->id?)
#define SETUNSP(shader, type, uname, value) \
	glUniform##type(glGetUniformLocation((shader).id, uname), value)

#define SETUNSP2(shader, type, uname, value1, value2) \
	glUniform##type(glGetUniformLocation((shader).id, uname), value1, value2)

#define SETUNSPM(shader, type, uname, value) \
	glUniformMatrix##type(glGetUniformLocation((shader).id, uname), 1, GL_FALSE, value)

#define SETUNSPMCT(shader, type, uname, value, count, transpose) \
	glUniformMatrix##type(glGetUniformLocation((shader).id, uname), count, transpose, value)

namespace agl {
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
}

