#pragma once

#include <stdint.h>
#include <memory>
#include <string>
#include <unordered_map>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Dimensional.h"
#include "GLFWApplication.h"
#include "Scene.h"

namespace agl {
	class ScenedGLFWApplication : public GLFWApplication {
	public:
    using GLFWApplication::GLFWApplication;
		void initialise() override;
		void tick() override;
		void readKeys() override;
		void onMouse(double xpos, double ypos) override;
		Scene* insertScene(std::string nm, std::unique_ptr<Scene>&& sc);
		void changeScene(std::string nm);
	private:
		std::unordered_map<std::string, std::unique_ptr<Scene>> scenes;
		std::string currentSceneName;
		friend class Scene;
	};
}