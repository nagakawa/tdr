#pragma once

#include <stdint.h>

namespace agl {
  class ScenedGLFWApplication;
  class Scene {
  public:
    void setDimensions(
      int width, int height, int actualWidth, int actualHeight);
    void setApp(ScenedGLFWApplication* a) { app = a; }
    virtual void initialise();
    virtual void update();
    virtual void render();
    virtual ~Scene();
  protected:
    int w, h, aw, ah;
    ScenedGLFWApplication* app;
  };
}