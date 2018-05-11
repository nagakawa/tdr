#pragma once

#include <stddef.h>
#include <memory>
#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>
// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "BlendMode.h"
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
	extern const char* S2D_VERTEX_SOURCE;
	extern const char* S2D_FRAGMENT_SOURCE;
	struct Sprite2DInfo {
		Rect source;
		Rect dest;
	};
	class Sprite2D: public Renderable {
	public:
		Sprite2D(Texture* t);
		~Sprite2D();
		void setUp();
		void tick();
		void update();
		int addSprite(Sprite2DInfo loc);
		void setTexture(Texture* tex);
		void setApp(Dimensional* a) { app = a; }
		Sprite2DInfo get(int index) { return sprites[index]; }
		Sprite2DInfo* getLoc(int index) { return sprites.data() + index; }
	protected:
		void _tearDown();
	private:
		Texture* texture;
		VBO vbo;
		VBO instanceVBO;
		VAO vao;
		ShaderProgram program;
		std::vector<Sprite2DInfo> sprites;
		Dimensional* app = nullptr;
		bool hasSetUniforms;
		bool hasInitialisedProgram;
		bool hasDeducedScreenDimensions = false;
		GLint vp[4];
	};
}
