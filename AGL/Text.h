#pragma once
#include "Renderable.h"

#define GLEW_STATIC
#include <GL/glew.h>
// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// Pango
extern "C" {
	#include <pango/pangocairo.h>
}

#include <cstddef>
#include <memory>
#include <string>
#include <vector>
#include "BlendMode.h"
#include "EBO.h"
#include "GLFWApplication.h"
#include "Renderable.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "VAO.h"
#include "VBO.h"
#include "cairoutil.h"
#include "rect.h"

namespace agl {
	extern const char* TXT_VERTEX_SOURCE;
	extern const char* TXT_FRAGMENT_SOURCE;
	class Text : public Renderable {
	public:
		Text();
		~Text();
		std::string getText() { return text; }
		void setText(std::string txt);
		std::string getFont() { return font; }
		void setFont(std::string fnt) { font = fnt; }
		unsigned int getWidth() { return width; }
		unsigned int getHeight() { return height; }
		unsigned int getMargin() { return margin; }
		void setMargin(unsigned int m) { margin = m; }
		double getSize() { return size; }
		void setSize(double s) { size = s; }
		void setUp();
		void tick();
		void update();
		glm::vec4 getTopColor() { return topColor; }
		void setTopColor(glm::vec4 col) { topColor = col; }
		glm::vec4 getBottomColor() { return bottomColor; }
		void setBottomColor(glm::vec4 col) { bottomColor = col; }
		void setColor(glm::vec4 col) {
			topColor = col;
			bottomColor = col;
		}
		glm::vec2 getPosition() { return position; }
		void setPosition(glm::vec2 pos) { position = pos; }
		void setApp(GLFWApplication* a) { app = a; }
	protected:
		void _tearDown();
	private:
		VBO vbo;
		VAO vao;
		ShaderProgram program;
		Texture* texture;
		std::string text;
		unsigned int width, height;
		GLFWApplication* app;
		bool hasInitializedProgram;
		glm::vec4 topColor;
		glm::vec4 bottomColor;
		glm::vec2 position;
		std::string font;
		unsigned int margin;
		double size;
	};
}
