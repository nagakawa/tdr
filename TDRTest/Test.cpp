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
#include <ScenedGLFWApplication.h>
#include <Shader.h>
#include <ShaderProgram.h>

#include <Bullet.h>
#include <Game.h>

// Others
#include <iostream>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cmath>

#ifdef _MSC_VER
#pragma warning(disable: 4099)
#endif

class TDRTest : public tdr::Game {
public:
	using tdr::Game::Game;
	void mainLoop() override {
		// Work
		readKeys();
		tdr::BulletList& bl = getBulletList();
		bl.createShotA1(50, 50, 3, kfp::frac32::raw(0x14828459u), bl.shotsheet.getRectByID(0), 0);
	}
	void readKeys() {
		if (testKey(GLFW_KEY_ESCAPE))
			glfwSetWindowShouldClose(app->underlying(), GL_TRUE);
		// More to come...
	}
	~TDRTest() {
		// Nothing yet?
	}
private:
};

void test() {
	std::cout << "Starting TDRTest...\n";
	agl::ScenedGLFWApplication* app = new agl::ScenedGLFWApplication(
		1280, 960, 0, 0, u8"TDR Test App", 4, 5, true);
	tdr::Shotsheet s;
	s.t = agl::Texture("textures-tdr/shotsheet.png");
	s.rectsByID[0] = {
		/*.texCoords = */ {0, 0, 16, 16},
		/* .visualRadius = */ 8,
		/* .collisionRadius = */ 4,
	};
	s.rectsByID[1] = {
		/*.texCoords = */ {16, 0, 32, 16},
		/* .visualRadius = */ 8,
		/* .collisionRadius = */ 4,
	};
	app->insertScene("Ultima loves MS", std::make_unique<TDRTest>(
		std::move(s),
		500, 800, 80, 80
	));
	app->changeScene("Ultima loves MS");
	app->start();
	delete app;
}

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
		test();
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
