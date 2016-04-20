#pragma once

#define GLEW_STATIC
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
	extern const char* VERTEX_SOURCE;
	extern const char* FRAGMENT_SOURCE;
	const GLfloat vertices[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f
	};
	struct Sprite2DInfo {
		Rect source;
		Rect dest;
	};
	class Sprite2D: public Renderable {
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
		Sprite2DInfo* getLoc(int index) { return sprites->data() + index; }
	protected:
		void _tearDown();
	private:
		std::shared_ptr<Texture> texture;
		VBO* vbo;
		VBO* instanceVBO;
		VAO* vao;
		ShaderProgram* program;
		std::vector<Sprite2DInfo>* sprites;
		GLFWApplication* app;
		bool hasSetUniforms;
	};
}
