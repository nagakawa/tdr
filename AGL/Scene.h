#pragma once

#include <stdint.h>

#include <GLFWApplication.h>

namespace agl {
  class ScenedGLFWApplication;
  class Scene {
  public:
    void setDimensions(
      int width, int height, int actualWidth, int actualHeight);
    void setApp(ScenedGLFWApplication* a) { app = a; }
    virtual void initialise() = 0;
    virtual void update() = 0;
    virtual void render() = 0;
    virtual ~Scene() = 0;
  protected:
    int w, h, aw, ah;
		bool testKey(int code);
		KeyStatus testKey2(int code);
		void setVSyncEnable(bool enable);
    ScenedGLFWApplication* app;
  };
}