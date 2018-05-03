#pragma once

#include <cstdio>
#include <cstdlib>
#include <GL/glew.h>

namespace agl {
	class Shader {
	public:
		Shader(const char* source, GLenum type);
		Shader(const char* source, size_t len, GLenum type);
		Shader(FILE* f, GLenum type);
		~Shader();
		GLuint id;
		GLenum shaderType;
	private:
		void init(const char* source, size_t len, GLenum type);
	};

	Shader* openShaderFromFile(const char* fname, GLenum type);
}