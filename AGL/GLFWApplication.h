#pragma once

#include <stdint.h>

#define DEFAULT_WIDTH 800
#define DEFAULT_HEIGHT 600
#define DEFAULT_TITLE "GLFWApplication"

class GLFWApplication {
public:
	GLFWApplication(
		int width = DEFAULT_WIDTH,
		int height = DEFAULT_HEIGHT,
		int actualWidth = 0,
		int actualHeight = 0,
		const char* title = DEFAULT_TITLE
	);
	~GLFWApplication();
	virtual void initialize();
	virtual bool tick();
	virtual void readKeys();
protected:
	bool testKey(int code);
private:
	uint64_t keys[16];
	void setKey(int code);
	void resetKey(int code);
};
