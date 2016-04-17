#pragma once

#include <cstdio>
#include <cstdlib>
#include <GL/glew.h>

class Shader {
public:
	Shader(const char* source, GLenum type);
	Shader(FILE* f, GLenum type);
	~Shader();
	GLuint id;
	GLenum shaderType;
private:
};

void openShaderFromFile(const char* fname, GLenum type, Shader& s);