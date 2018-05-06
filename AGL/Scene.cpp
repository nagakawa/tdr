#include "Scene.h"

#include <ScenedGLFWApplication.h>

namespace agl {
  void Scene::setDimensions(
      int width, int height, int actualWidth, int actualHeight) {
    w = width;
    h = height;
    aw = actualWidth;
    ah = actualHeight;
  }
	bool Scene::testKey(int code) {
    return app->testKey(code);
  }
  KeyStatus Scene::testKey2(int code) {
    return app->testKey2(code);
  }
  void Scene::setVSyncEnable(bool enable) {
    app->setVSyncEnable(enable);
  }
  Scene::~Scene() {}
}