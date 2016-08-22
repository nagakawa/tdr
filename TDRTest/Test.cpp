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

#include "Collidable.h"

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
	std::cout << "Fixed-point function test: trigonometry\n";
	int32_t c, s;
	tdr::fix1616 cf, sf, r, t;
	tdr::fix1616 i = tdr::C_ZERO;
	do {
		tdr::sincos(i, c, s);
		printf("%8x -> (%8x, %8x)", i.u, c, s);
		cf.u = c >> 14;
		sf.u = s >> 14;
		tdr::rectp(cf, sf, r, t);
		printf(" -> %8x < %8x\n", r.u, t.u);
		i.u += 0x1000000;
	} while (i != tdr::C_ZERO);
}
