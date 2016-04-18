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

// Others
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>

#ifdef _MSC_VER
#pragma warning(disable: 4099)
#endif

class AGLTest;

class Boxes : Renderable {
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
	Texture* container;
	Texture* awesome;
	VBO* vbo;
	EBO* ebo;
	VAO* vao;
	ShaderProgram* program;
	AGLTest* app;
};

class AGLTest : GLFWApplication {
public:
	using GLFWApplication::GLFWApplication;
	void initialize() {
		std::cout << "hi\n";
		boxes = new Boxes(this);
	}
	void tick() {
		glClearColor(0.5f, 0.7f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		boxes->tick();
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
	GLfloat getMix() { return mix; }
	GLfloat mix = 0.0f;
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	GLfloat deltaTime = 0.0f;
	GLfloat lastFrame = 0.0f;
	GLfloat lastX = 400, lastY = 300;
	GLfloat yaw = 0, pitch = 0;
	bool firstMouse = true;
	GLfloat fov = 45.0f;
	Boxes* boxes;
};

Boxes::Boxes(AGLTest* a) {
	app = a;
	setUp();
}
void Boxes::setUp() {
	container = new Texture("textures/container.jpg");
	awesome = new Texture("textures/awesomeface.png");
	vbo = new VBO();
	ebo = new EBO();
	Shader* vertexShader = openShaderFromFile("shader/vertex.glsl", GL_VERTEX_SHADER);
	Shader* fragmentShader = openShaderFromFile("shader/fragment.glsl", GL_FRAGMENT_SHADER);
	program = new ShaderProgram();
	program->attach(*vertexShader);
	program->attach(*fragmentShader);
	program->link();
	program->use();
	delete vertexShader;
	delete fragmentShader;
	vao = new VAO();
	vao->setActive();
	vbo->feedData(sizeof(vertices), vertices, GL_STATIC_DRAW);
	ebo->feedData(sizeof(indices), indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*) 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*) (3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
}
void Boxes::tick() {
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
	projection = glm::perspective(glm::radians(app->fov), 800.0f / 600.0f, 0.1f, 100.0f);
	for (int i = 0; i < 10; ++i) {
		glm::mat4 model;
		model = glm::translate(model, cubePositions[i]);
		model = glm::rotate(model, glm::radians((GLfloat) ((i % 3 == 0 ? glfwGetTime() * 50.0f : 0) + i * 20.0f)), glm::vec3(0.5f, 1.0f, 0.0f));
		SETUNSPM(*program, 4fv, "model", glm::value_ptr(model));
		SETUNSPM(*program, 4fv, "view", glm::value_ptr(view));
		SETUNSPM(*program, 4fv, "projection", glm::value_ptr(projection));
		vao->setActive();
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	}
}

int main(int argc, char** argv) {
	try {
		// Kriët ė test wýndö
		AGLTest* a = new AGLTest(800, 600, 0, 0, u8"テスト");
		a->start();
	} catch (char* s) {
		std::cout << u8"An error has Okuued!\n\n" << s << u8"\n\n";
		getchar();
	}
}