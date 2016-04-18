#pragma once

#include <stdint.h>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define DEFAULT_WIDTH 800
#define DEFAULT_HEIGHT 600
#define DEFAULT_TITLE "GLFWApplication"
#define ROLLING_FRAME_COUNT 30

/*
	GLFWApplication: a class for applications.

	Users should create subclasses of GLFWApplication
	for their own purposes.
*/
class GLFWApplication {
public:
	GLFWApplication(
		int width = DEFAULT_WIDTH,
		int height = DEFAULT_HEIGHT,
		int actualWidth = 0,
		int actualHeight = 0,
		const char* title = DEFAULT_TITLE
	);
	void start();
	~GLFWApplication();
	virtual void initialize();
	virtual void tick();
	virtual void readKeys();
	virtual void onMouse(double xpos, double ypos);
	// These methods must be public in order to be
	// referenceable by keyCallback.
	void setKey(int code);
	void resetKey(int code);
	double getFPS() { return fps; }
	double getDelta() { return delta; }
	double getRollingFPS() { return rollingFPS; }
	GLFWwindow* underlying() { return window; }
protected:
	bool testKey(int code);
private:
	uint64_t keys[16];
	GLFWwindow* window;
	GLdouble fps;
	GLdouble rollingFPS;
	GLdouble delta;
	GLdouble currentTime;
	GLdouble cumulDelta;
	GLint currInRF;
};

extern GLFWApplication* currentApp;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
