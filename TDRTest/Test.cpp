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
		tdr::Shotsheet s;
		s.t = agl::Texture("textures-tdr/shotsheet.png");
		s.rectsByID[0] = {
			/*.texCoords = */ {0, 0, 16, 16},
			/* .visualRadius = */ 16,
			/* .collisionRadius = */ 8,
		};
		s.rectsByID[1] = {
			/*.texCoords = */ {16, 0, 32, 16},
			/* .visualRadius = */ 16,
			/* .collisionRadius = */ 8,
		};
		g = std::make_unique<tdr::Game>(
			this, std::move(s), getWidth(), getHeight());
		glfwSetWindowTitle(underlying(), "TDRTest");
	}
	void tick() {
		// Work
		g->update();
		tdr::BulletList& bl = g->getBulletList();
		bl.createShotA1(50, 50, 1, kfp::frac32::raw(0x94828459u), bl.shotsheet.getRectByID(0), 0);
		// Render
		glClearColor(0.5f, 0.7f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
