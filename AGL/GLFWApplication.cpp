#include "GLFWApplication.h"

#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace agl;

GLFWApplication* agl::currentApp;

#include "begincbackdecl.h"
// Thanks learnopengl.com!
void APIENTRY glDebugOutput(GLenum source,
                            GLenum type,
                            GLuint id,
                            GLenum severity,
                            GLsizei length,
                            const GLchar *message,
                            const void *userParam) {
  // ignore non-significant error/warning codes
  if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;
  std::cout << "---------------" << std::endl;
  std::cout << "Debug message " << id << " (" << gluErrorString(id) << "): " <<  message << std::endl;
  switch (source) {
    case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
    case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
    case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
  }
	std::cout << std::endl;
  switch (type) {
    case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
    case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
    case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
    case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
    case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
    case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
    case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
  }
	std::cout << std::endl;
  switch (severity) {
    case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
    case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
    case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
    case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
  }
	std::cout << std::endl;
  std::cout << std::endl;
}
#include "endcbackdecl.h"

GLFWApplication::GLFWApplication(
	int width,
	int height,
	int actualWidth,
	int actualHeight,
	const char* title,
	int glMajor,
	int glMinor,
	bool debug,
  int maxFPS
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
  aw = actualWidth;
  ah = actualHeight;
  mfps = maxFPS;
	projection = glm::scale(projection, glm::vec3(1.0f / width, 1.0f / height, 1));
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
		throw u8"Failed to initialise GLEW";
	}
	glGetError();
	if (debug) {
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
		GLint flags;
		glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
		if (flags & GL_CONTEXT_FLAG_DEBUG_BIT || glMajor >= 5 || (glMajor == 4 && glMinor >= 3)) {
		    // initialise debug output
				glEnable(GL_DEBUG_OUTPUT);
		    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		    glDebugMessageCallback(glDebugOutput, nullptr);
		    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
		} else {
			std::cout << "Warning: requested debug output but we can't do it\n";
		}
	}
	glViewport(0, 0, width, height);
	glEnable(GL_MULTISAMPLE);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouseCallback);
	memset(keys, 0, sizeof(uint64_t) * 16);
	setVSyncEnable(false);
}

#ifdef __WIN32
#include <windows.h>
inline void wait(double t) {
  Sleep((int) (t * 1000));
}
#else
#include <time.h>
inline void wait(double t) {
  struct timespec rqtp;
  rqtp.tv_sec = (int) (t / 1000000000);
  rqtp.tv_nsec = (int) (t * 1000000000) % 1000000000;
  nanosleep(&rqtp, nullptr);
}
#endif

#define WAITING_DISTANCE 0.5
#define THRESH 0.002

void GLFWApplication::start() {
	currentTime = glfwGetTime();
	initialise();
	while (!glfwWindowShouldClose(window)) {
		GLdouble prevTime = currentTime;
		currentTime = glfwGetTime();
		// Keep at slightly more than maxFPS so FPS doesn't dip below as often
		double goal = prevTime + 0.999 / mfps;
    		  while (currentTime < goal) {
    		  currentTime = glfwGetTime();
    		  double diff = goal - currentTime;
    		  if (diff < THRESH) wait(WAITING_DISTANCE * diff);
    		}
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
		glFinish();
	}
}

GLFWApplication::~GLFWApplication() {
	glfwSetWindowShouldClose(underlying(), true);
}

void GLFWApplication::initialise() {
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
