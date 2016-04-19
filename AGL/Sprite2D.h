#pragma once

#include <GL/glew.h>
// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cstddef>
#include <memory>
#include <vector>
#include "EBO.h"
#include "GLFWApplication.h"
#include "Renderable.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "VAO.h"
#include "VBO.h"
#include "rect.h"

namespace agl {
	const char* VERTEX_SOURCE = "\
#version 330 core \n\
layout (location = 0) in vec4 source; \
layout (location = 1) in vec4 dest; \
layout (location = 2) in vec2 pos; \
out vec2 texCoord; \
uniform vec2 texDimensions; \
uniform vec2 screenDimensions; \
\
void main() { \
	texCoord = vec2(mix(source.x, source.z, pos.x), mix(source.y, source.w, pos.y)) / texDimensions; \
	gl_Position = vec4(mix(dest.x, dest.z, pos.x), mix(dest.y, dest.w, pos.y), 0.0f, 1.0f) / vec4(screenDimensions, 1.0f, 1.0f); \
} \
";
	const char* FRAGMENT_SOURCE = "\
#version 330 core \n\
in vec2 texCoord; \
out vec4 color; \
uniform sampler2D tex; \
\
void main() { \
	color = texture(tex, texCoord); \
} \
";
	const GLfloat vertices[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f
	};
	const GLint elements[] = { 0, 1, 2, 1, 2, 3 };
	struct Sprite2DInfo {
		Rect source;
		Rect dest;
	};
	class Sprite2D: Renderable {
	public:
		Sprite2D(std::shared_ptr<Texture> t);
		~Sprite2D();
		void setUp();
		void tick();
		void update();
		int addSprite(Sprite2DInfo loc);
		void setTexture(std::shared_ptr<Texture> tex);
		void setApp(GLFWApplication* a) { app = a; }
		Sprite2DInfo get(int index) { return (*sprites)[index]; }
	protected:
		void _tearDown();
	private:
		std::shared_ptr<Texture> texture;
		VBO* vbo;
		VBO* instanceVBO;
		VAO* vao;
		EBO* ebo;
		ShaderProgram* program;
		std::vector<Sprite2DInfo>* sprites;
		GLFWApplication* app;
	};
}
