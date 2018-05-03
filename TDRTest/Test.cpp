// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
// SOIL
#include <SOIL.h>
// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Test.h>
#include <GLFWApplication.h>
#include <Renderable.h>
#include <Texture.h>
#include <VAO.h>
#include <VBO.h>
#include <EBO.h>
#include <Shader.h>
#include <ShaderProgram.h>

#include "Bullet.h"
#include "Game.h"

// Others
#include <iostream>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cmath>

#ifdef _MSC_VER
#pragma warning(disable: 4099)
#endif

class TDRTest : public agl::GLFWApplication {
public:
	using agl::GLFWApplication::GLFWApplication;
	void initialise() {
		std::cout << "Starting TDRTest...\n";
		g = std::make_unique<tdr::Game>(this, getWidth(), getHeight());
	}
	void tick() {
		g->update();
		g->render();
	}
	void readKeys() {
		if (testKey(GLFW_KEY_ESCAPE))
			glfwSetWindowShouldClose(underlying(), GL_TRUE);
		// 
	}
	void start() {
		GLFWApplication::start();
	}
	~TDRTest() {
		// Nothing yet?
	}
private:
	std::unique_ptr<tdr::Game> g;
};

static void glfwError(int id, const char* description) {
	(void) id;
  std::cerr << description << std::endl;
}

int main(int argc, char** argv) {
	(void) argc;
	(void) argv;
	glfwSetErrorCallback(glfwError);
	glfwInit();
	try {
		TDRTest* a = new TDRTest(800, 600, 0, 0, u8"TDR Test App", 4, 5, true);
		a->start();
	} catch (char const* s) {
		std::cerr << u8"An error has Okuued!\n\n" << s << u8"\n\n";
		return -1;
	} catch (char* s) {
		std::cerr << u8"An error has Okuued!\n\n" << s << u8"\n\n";
		return -1;
	} catch (const std::string& s) {
		std::cerr << u8"An error has Okuued!\n\n" << s << u8"\n\n";
		return -1;
	}
	glfwTerminate();
	return 0;
}
