#pragma once

#include <stddef.h>
#include <memory>
#include <string>
#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>
// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "BlendMode.h"
#include "Dimensional.h"
#include "EBO.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "VAO.h"
#include "VBO.h"
#include "rect.h"
#include "text/Font.h"
#include "text/GlyphDrawList.h"

namespace agl {
  class XText {
  public:
    XText(const LayoutInfo& l) : layout(l) {}
    LayoutInfo& getLayout() { return layout; }
    void setApp(Dimensional* a) { app = a; }
    void setUp();
    void render();
    void setText(std::string&& s);
  private:
    struct RInfo {
      Rect bounds;
      glm::vec2 pos;
    };
    void spurt(std::vector<GlyphInfo>& gis);
    void update();
    void setUniforms1();
    void setUniforms2(size_t page);
    std::vector<std::vector<RInfo>> rinfo;
    std::vector<size_t> offsets;
    bool hasInitialisedProgram = false;
    bool hasSetUniforms = false;
    glm::vec2 dimensions;
		glm::vec2 position;
    LayoutInfo layout;
    std::string text;
		VBO vbo;
    VBO instanceVBO;
		VAO vao;
		ShaderProgram program;
		Dimensional* app;
  };
}
