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
#include "Renderable.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "VAO.h"
#include "VBO.h"
#include "rect.h"

namespace agl {
	const char* VERTEX_SOURCE = "\
#version 330 core \
layout (location = 0) in vec4 source; \
layout (location = 1) in vec4 dest; \
layout (location = 2) in vec2 pos; \
out vec2 texCoord; \
\
void main() { \
	texCoord = vec2(source.x + pos.x * (source.z - source.x), source.y + pos.y * (source.w - source.y)); \
	gl_Position = vec4(dest.x + pos.x * (dest.z - dest.x), dest.y + pos.y * (dest.w - dest.y), 0.0f, 1.0f); \
} \
";
	const char* FRAGMENT_SOURCE = "\
#version 330 core \
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
	};
}
