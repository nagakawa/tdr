#include "GLFWApplication.h"

#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

GLFWApplication* currentApp;

GLFWApplication::GLFWApplication(
	int width,
	int height,
	int actualWidth,
	int actualHeight,
	const char* title
	) {
	// Set actual width and height of window to whatever
	// you're working on if not set
	if (actualWidth == 0) actualWidth = width;
	if (actualHeight == 0) actualHeight = height;
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	window = glfwCreateWindow(actualWidth, actualHeight, title, nullptr, nullptr);
	if (window == nullptr) {
		glfwTerminate();
		throw "Failed to create GLFW window";
	}
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		throw "Failed to initialize GLEW";
	}
	glViewport(0, 0, width, height);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouseCallback);
}

void GLFWApplication::start() {
	currentTime = glfwGetTime();
	initialize();
	while (!glfwWindowShouldClose(window)) {
		GLdouble prevTime = currentTime;
		currentTime = glfwGetTime();
		delta = currentTime - prevTime;
		fps = 1.0 / delta;
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

void GLFWApplication::setKey(int code) {
	keys[code >> 6] |= (1LL << (code & 63));
}

void GLFWApplication::resetKey(int code) {
	keys[code >> 6] &= ~(1LL << (code & 63));
}

#include "begincbackdecl.h"

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (action == GLFW_PRESS) currentApp->setKey(key);
	else if (action == GLFW_RELEASE) currentApp->resetKey(key);
}

void GLFWApplication::onMouse(double xpos, double ypos) {
	return;
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
	currentApp->onMouse(xpos, ypos);
}

#include "endcbackdecl.h"
