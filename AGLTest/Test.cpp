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

#include "Test.h"
#include "GLFWApplication.h"
#include "Renderable.h"
#include "Texture.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "Sprite2D.h"
#include "Text.h"

// Others
#include <iostream>
#include <memory>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <cmath>

#ifdef _MSC_VER
#pragma warning(disable: 4099)
#endif

class AGLTest;

class Boxes : agl::Renderable {
public:
	Boxes(AGLTest* a);
	void setUp();
	void tearDown() {
		delete container;
		delete awesome; // aww
		delete vbo;
		delete ebo;
		delete vao;
		delete program;
	}
	void tick();
	void update() {
	}
private:
	agl::Texture* container;
	agl::Texture* awesome;
	agl::VBO* vbo;
	agl::EBO* ebo;
	agl::VAO* vao;
	agl::ShaderProgram* program;
	AGLTest* app;
};

#define DIGIT_WIDTH 14.8f
#define DIGIT_HEIGHT 18
#define DIGIT_BASEX 16
#define DIGIT_BASEY 230

class AGLTest : public agl::GLFWApplication {
public:
	using agl::GLFWApplication::GLFWApplication;
	void initialize() {
		std::cout << "hi\n";
		boxes = new Boxes(this);
		ptex = new agl::Texture("textures/fuckyou.png");
		stex = std::make_shared<agl::Texture>(*ptex);
		sprites = new agl::Sprite2D(stex);
		sprites->setApp(this);
		sprites->addSprite({
			{0, 448, 512, 512},
			{0, 0, 512, 64}
		});
		sprites->addSprite({
			{DIGIT_BASEX, DIGIT_BASEY, DIGIT_BASEX + 4 * DIGIT_WIDTH, DIGIT_BASEY + DIGIT_HEIGHT},
			{50, 500, 50 + 4 * DIGIT_WIDTH, 500 + DIGIT_HEIGHT}
		});
		sprites->addSprite({
			{ DIGIT_BASEX + 14 * DIGIT_WIDTH, DIGIT_BASEY, DIGIT_BASEX + 15 * DIGIT_WIDTH, DIGIT_BASEY + DIGIT_HEIGHT },
			{50 + 6 * DIGIT_WIDTH, 500, 50 + 7 * DIGIT_WIDTH, 500 + DIGIT_HEIGHT}
		});
		for (int i = 0; i < 4; ++i) {
			GLfloat x = 50 + (4 + i + (i >= 2)) * DIGIT_WIDTH;
			sprites->addSprite({
				{0, DIGIT_BASEY, 0, DIGIT_BASEY + DIGIT_HEIGHT},
				{x, 500, x + DIGIT_WIDTH, 500 + DIGIT_HEIGHT}
			});
		}
		sprites->setUp();
		fy = new agl::Text();
		fy->setApp(this);
		fy->setFont("Meiryo");
		fy->setText(u8"Test application.\nぱんご");
		fy->setPosition(glm::vec2(530, 30));
		fy->setUp();
	}
	bool ff = true;
	void tick() {
		glClearColor(0.5f, 0.7f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		++frame;
		int which = (frame >> 6) & 3;
		sprites->getLoc(0)->source.top = (GLfloat) (448 - (which << 6));
		sprites->getLoc(0)->source.bottom = (GLfloat) (512 - (which << 6));
		int rfps = (int) (getRollingFPS() * 100);
		for (int i = 3; i >= 0; --i) {
			setDigit(i, rfps % 10);
			rfps /= 10;
		}
		sprites->update();
		boxes->tick();
		sprites->tick();
		fy->tick();
		char* ctitle = new char[256];
		snprintf(ctitle, 255, "TestApp @ GL %s | FPS: %lf", glGetString(GL_VERSION), getRollingFPS());
		glfwSetWindowTitle(underlying(), ctitle);
	}
	void readKeys() {
		// Test if window should close
		if (testKey(GLFW_KEY_ESCAPE))
			glfwSetWindowShouldClose(underlying(), GL_TRUE);
		// Camera controls
		GLfloat cameraSpeed = 5.0f * (GLfloat) getDelta();
		if (testKey(GLFW_KEY_W))
			cameraPos += cameraSpeed * cameraFront;
		if (testKey(GLFW_KEY_S))
			cameraPos -= cameraSpeed * cameraFront;
		if (testKey(GLFW_KEY_A))
			cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		if (testKey(GLFW_KEY_D))
			cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		// Slider controls
		if (testKey(GLFW_KEY_UP)) mix += 0.005f;
		if (testKey(GLFW_KEY_DOWN)) mix -= 0.005f;
	}
	void onMouse(double xpos, double ypos) {
		if (firstMouse) {
			lastX = (GLfloat) xpos;
			lastY = (GLfloat) ypos;
			firstMouse = false;
		}
		GLfloat xoffset = (GLfloat) xpos - lastX;
		GLfloat yoffset = lastY - (GLfloat) ypos;
		lastX = (GLfloat) xpos;
		lastY = (GLfloat) ypos;
		GLfloat sensitivity = 0.10f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;
		yaw += xoffset;
		pitch += yoffset;
		if (pitch > 89.0f) pitch = 89.0f;
		if (pitch < -89.0f) pitch = -89.0f;
		glm::vec3 front;
		front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
		front.y = sin(glm::radians(pitch));
		front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
		cameraFront = glm::normalize(front);
	}
	void start() {
		GLFWApplication::start();
	}
	~AGLTest() {
		delete boxes;
		delete sprites;
		delete fy;
	}
	GLfloat getMix() { return mix; }
	GLfloat mix = 0.0f;
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	GLfloat deltaTime = 0.0f;
	GLfloat lastFrame = 0.0f;
	GLfloat lastX = 400, lastY = 300;
	GLfloat yaw = 90, pitch = 0;
	bool firstMouse = true;
	GLfloat fov = 45.0f;
	Boxes* boxes;
	std::shared_ptr<agl::Texture> stex;
	agl::Texture* ptex;
	agl::Sprite2D* sprites;
	agl::Text* fy;
	int frame = 0;
	void setDigit(int i, int v) {
		agl::Sprite2DInfo* spr = sprites->getLoc(3 + i);
		spr->source.left = DIGIT_BASEX + (4 + v) * DIGIT_WIDTH;
		spr->source.right = DIGIT_BASEX + (5 + v) * DIGIT_WIDTH;
	}
};

Boxes::Boxes(AGLTest* a) {
	app = a;
	setUp();
}
void Boxes::setUp() {
	container = new agl::Texture("textures/container.jpg");
	awesome = new agl::Texture("textures/awesomeface.png");
	vbo = new agl::VBO();
	ebo = new agl::EBO();
	agl::Shader* vertexShader = agl::openShaderFromFile("shader/vertex.glsl", GL_VERTEX_SHADER);
	agl::Shader* fragmentShader = agl::openShaderFromFile("shader/fragment.glsl", GL_FRAGMENT_SHADER);
	program = new agl::ShaderProgram();
	program->attach(*vertexShader);
	program->attach(*fragmentShader);
	program->link();
	program->use();
	delete vertexShader;
	delete fragmentShader;
	vao = new agl::VAO();
	vao->setActive();
	vbo->feedData(sizeof(vertices), vertices, GL_STATIC_DRAW);
	ebo->feedData(sizeof(indices), indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*) 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*) (3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
}
void Boxes::tick() {
	glDisable(GL_BLEND);
	vao->setActive();
	program->use();
	glEnable(GL_DEPTH_TEST);
	container->bindTo(0);
	SETUNSP(*program, 1i, "ourTexture", 0);
	awesome->bindTo(1);
	SETUNSP(*program, 1i, "ourTexture2", 1);
	SETUNSP(*program, 1f, "m", app->mix);
	GLfloat radius = 10.0f;
	GLfloat camX = (GLfloat) sin(glfwGetTime()) * radius;
	GLfloat camZ = (GLfloat) cos(glfwGetTime()) * radius;
	glm::mat4 view;
	view = glm::lookAt(
		app->cameraPos, // position
		app->cameraPos + app->cameraFront, // target
		app->cameraUp); // up
	glm::mat4 projection;
	GLfloat aspect = ((GLfloat) app->getWidth()) / app->getHeight();
	projection = glm::perspective(glm::radians(app->fov), aspect, 0.1f, 100.0f);
	for (int i = 0; i < 10; ++i) {
		glm::mat4 model;
		model = glm::translate(model, cubePositions[i]);
		model = glm::rotate(model, glm::radians((GLfloat) ((i % 3 == 0 ? glfwGetTime() * 50.0f : 0) + i * 20.0f)), glm::vec3(0.5f, 1.0f, 0.0f));
		SETUNSPM(*program, 4fv, "model", glm::value_ptr(model));
		SETUNSPM(*program, 4fv, "view", glm::value_ptr(view));
		SETUNSPM(*program, 4fv, "projection", glm::value_ptr(projection));
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	}
}

int main(int argc, char** argv) {
	try {
		// Kriët ė test wýndö
		AGLTest* a = new AGLTest(800, 600, 0, 0, u8"AGL Test App");
		a->start();
	} catch (char* s) {
		std::cout << u8"An error has Okuued!\n\n" << s << u8"\n\n";
		getchar();
	}
}