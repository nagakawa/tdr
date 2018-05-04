#include "Scene.h"

namespace agl {
  void Scene::setDimensions(
      int width, int height, int actualWidth, int actualHeight) {
    w = width;
    h = height;
    aw = actualWidth;
    ah = actualHeight;
  }
}