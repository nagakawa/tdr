#include "Shader.h"

#include "cross.h"

using namespace agl;

Shader::Shader(const char* source, GLenum type) {
	init(source, type);
}

void Shader::init(const char* source, GLenum type) {
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
		throw u8"Tried to pass in a null pointer!";
	}
	// Slurp the source
	// Thanks http://stackoverflow.com/questions/14002954/c-programming-how-to-read-the-whole-file-contents-into-a-buffer
	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	fseek(f, 0, SEEK_SET);
	char* string = (char*) malloc(fsize+1);
	size_t br = fread(string, fsize, 1, f);
	fclose(f);
	if (br < 1) throw u8"fread did not read as much as it should have";
	string[fsize] = 0;
	init(string, type);
}

Shader::~Shader() {
	glDeleteShader(id);
}

Shader* agl::openShaderFromFile(const char* fname, GLenum type) {
	FILE* f;
	openFile(&f, fname, "rb");
	if (f == nullptr) {
		fprintf(stderr, u8"File %s missing or unreadable!", fname);
		throw u8"nofile";
	}
	Shader* shader = new Shader(f, type);
	return shader;
}
