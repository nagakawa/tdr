#include "Shader.h"

#include "cross.h"

Shader::Shader(const char* source, GLenum type) {
	id = glCreateShader(type);
	glShaderSource(id, 1, &source, NULL);
	glCompileShader(id);
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(id, 512, NULL, infoLog);
		throw infoLog;
	}
	shaderType = type;
}

Shader::Shader(FILE* f, GLenum type) {
	if (f == nullptr) {
		throw "Tried to pass in a null pointer!";
	}
	// Slurp the source
	// Thanks http://stackoverflow.com/questions/14002954/c-programming-how-to-read-the-whole-file-contents-into-a-buffer
	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	fseek(f, 0, SEEK_SET);
	char* string = (char*) malloc(fsize+1);
	fread(string, fsize, 1, f);
	fclose(f);
	string[fsize] = 0;
	Shader(string, type);
}

Shader::~Shader() {
	glDeleteShader(id);
}

void openShaderFromFile(const char* fname, GLenum type, Shader& s) {
	FILE* f;
	fopen_s(&f, fname, "rb");
	if (f == nullptr) {
		fprintf(stderr, "File %s missing or unreadable!", fname);
		throw "nofile";
	}
	Shader shader(f, type);
	s = shader;
	fclose(f);
}