#include "ScenedGLFWApplication.h"

namespace agl {
  Scene* ScenedGLFWApplication::insertScene(
      std::string nm, std::unique_ptr<Scene>&& sc) {
    Scene* scp = sc.get();
    sc->initialise();
    sc->setApp(this);
    sc->setDimensions(
      getWidth(), getHeight(), getActualWidth(), getActualHeight());
    scenes[std::move(nm)] = std::move(sc);
    return scp;
  }
  void ScenedGLFWApplication::changeScene(std::string name) {
    currentSceneName = std::move(name);
  }
  void ScenedGLFWApplication::initialise() {
    GLFWApplication::initialise();
  }
  void ScenedGLFWApplication::tick() {
    GLFWApplication::tick();
    scenes.at(currentSceneName)->update();
    scenes.at(currentSceneName)->render();
  }
  void ScenedGLFWApplication::readKeys() {
    GLFWApplication::readKeys();
  }
  void ScenedGLFWApplication::onMouse(double xpos, double ypos) {
    GLFWApplication::onMouse(xpos, ypos);
  }
}