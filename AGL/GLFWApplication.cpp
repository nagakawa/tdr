#include "GLFWApplication.h"

#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace agl;

GLFWApplication* agl::currentApp;

GLFWApplication::GLFWApplication(
	int width,
	int height,
	int actualWidth,
	int actualHeight,
	const char* title,
	int glMajor,
	int glMinor
	) {
	// Set actual width and height of window to whatever
	// you're working on if not set
	if (actualWidth == 0) actualWidth = width;
	if (actualHeight == 0) actualHeight = height;
	currInRF = 0;
	rollingFPS = 60;
	cumulDelta = 0;
	w = width;
	h = height;
	projection = glm::scale(projection, glm::vec3(1.0f / width, 1.0f / height, 1));
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, glMajor);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, glMinor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	window = glfwCreateWindow(actualWidth, actualHeight, title, nullptr, nullptr);
	if (window == nullptr) {
		glfwTerminate();
		throw u8"Failed to create GLFW window";
	}
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		throw u8"Failed to initialize GLEW";
	}
	glViewport(0, 0, width, height);
	glEnable(GL_MULTISAMPLE);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouseCallback);
	setVSyncEnable(true);
}

void GLFWApplication::start() {
	currentTime = glfwGetTime();
	initialize();
	while (!glfwWindowShouldClose(window)) {
		GLdouble prevTime = currentTime;
		currentTime = glfwGetTime();
		delta = currentTime - prevTime;
		fps = 1.0 / delta;
		cumulDelta += delta;
		++currInRF;
		if (cumulDelta >= FPS_UPDATE_PERIOD) {
			rollingFPS = currInRF / cumulDelta;
			currInRF = 0;
			cumulDelta = 0;
		}
		currentApp = this;
		glfwPollEvents();
		readKeys();
		tick();
		glfwSwapBuffers(window);
	}
	delete this;
}

GLFWApplication::~GLFWApplication() {
	glfwTerminate();
}

void GLFWApplication::initialize() {
	return;
}

void GLFWApplication::tick() {
	return;
}

void GLFWApplication::readKeys() {
	return;
}

bool GLFWApplication::testKey(int code) {
	return (keys[code >> 6] >> (code & 63)) & 1;
}

void GLFWApplication::setVSyncEnable(bool enable) {
	glfwSwapInterval(enable);
}

void GLFWApplication::setKey(int code) {
	keys[code >> 6] |= (1LL << (code & 63));
}

void GLFWApplication::resetKey(int code) {
	keys[code >> 6] &= ~(1LL << (code & 63));
}

#include "begincbackdecl.h"

void agl::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (action == GLFW_PRESS) currentApp->setKey(key);
	else if (action == GLFW_RELEASE) currentApp->resetKey(key);
}

void GLFWApplication::onMouse(double xpos, double ypos) {
	return;
}

void agl::mouseCallback(GLFWwindow* window, double xpos, double ypos) {
	currentApp->onMouse(xpos, ypos);
}

#include "endcbackdecl.h"
