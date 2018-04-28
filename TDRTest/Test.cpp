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

// Others
#include <iostream>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cmath>

#ifdef _MSC_VER
#pragma warning(disable: 4099)
#endif

int main(int argc, char** argv) {
	(void) argc;
	(void) argv;
	std::cout << "Fixed-point function test: trigonometry\n";
	std::cout << "But this is already done in kozet_fixed_point,\n";
	std::cout << "so you can go fuck yourself!\n";
	return 0;
}
