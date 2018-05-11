#include "text/XText.h"

#include "embedShader.h"

EMBED_SHADER(xt_vertex_source, vertex, static)
EMBED_SHADER(xt_fragment_source, fragment, static)

namespace agl {
  void XText::setUp() {
    Shader vertex(vertexSource, vertexSourceSize - 1, GL_VERTEX_SHADER);
    Shader fragment(fragmentSource, fragmentSourceSize - 1, GL_FRAGMENT_SHADER);
    program.attach(vertex);
    program.attach(fragment);
    program.link();
    hasInitialisedProgram = true;
    vao.setActive();
	  vbo.feedData(
      sizeof(rectangleVertices), (void*) rectangleVertices, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
	  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*) 0);
    instanceVBO.setActive();
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
      1, 4, GL_FLOAT, false, sizeof(RInfo),
      (GLvoid*) (offsetof(RInfo, bounds))
    );
    glVertexAttribDivisor(1, 1);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(
      2, 2, GL_FLOAT, false, sizeof(RInfo),
      (GLvoid*) (offsetof(RInfo, pos))
    );
    glVertexAttribDivisor(2, 1);
    agl::resetVBO();
    agl::resetVAO();
  }
  void XText::spurt(std::vector<GlyphInfo>& gis) {
    // Spurts the render-related data from the glyph list
    // to the `rinfo` vector. This vector will then be used by
    // the render code to draw the glyphs.
    for (auto& ri : rinfo) ri.clear();
    rinfo.resize(layout.f->texs.size());
    for (const GlyphInfo& gi : gis) {
      uint32_t id = gi.index;
      Font::GlyphInfo& props = layout.f->getInfo(id);
      // Where should we push?
      auto& toPush = rinfo[props.texid];
      toPush.emplace_back();
      RInfo& entry = toPush.back();
      entry.bounds = props.box;
      entry.pos = { gi.x / 64.0f, gi.y / 64.0f };
    }
    // Update offsets vector
    offsets.resize(rinfo.size() + 1);
    offsets[0] = 0;
    for (size_t i = 0; i < rinfo.size(); ++i)
      offsets[1 + i] = offsets[i] + rinfo[i].size();
  }
  void XText::update() {
    instanceVBO.feedData(
      offsets.back() * sizeof(RInfo),
      nullptr,
      GL_STREAM_DRAW);
    // Set individual chunks
    for (size_t i = 0; i < rinfo.size(); ++i) {
      instanceVBO.feedSubdata(
        offsets[i] * sizeof(RInfo),
        (offsets[i + 1] - offsets[i]) * sizeof(RInfo),
        rinfo[i].data()
      );
    }
  }
  void XText::setUniforms1() {
    vao.setActive();
    program.use();
    if (hasSetUniforms) return;
    SETUNSP2(program, 2f, "screenDimensions",
      (GLfloat) app->getWidth(), (GLfloat) app->getHeight());
    hasSetUniforms = true;
  }
  void XText::setUniforms2(size_t page) {
    Texture& t = layout.f->texs[page];
    t.bindTo(0);
    SETUNSP(program, 1i, "tex", 0);
    SETUNSP2(program, 2f, "texDimensions",
      (GLfloat) t.getWidth(), (GLfloat) t.getHeight());
  }
  void XText::render() {
    if (!hasInitialisedProgram) setUp();
    vao.setActive();
    update();
    glEnable(GL_BLEND);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    BM_ALPHA.use();
    setUniforms1();
    for (size_t i = 0; i < rinfo.size(); ++i) {
      setUniforms2(i);
      // XXX: 4.2-ism here, will have to fix later
      glDrawArraysInstancedBaseInstance(
        GL_TRIANGLE_STRIP,
        0,
        4,
        offsets[i + 1] - offsets[i],
        offsets[i]
      );
    }
    agl::resetVAO();
  }
  void XText::setText(std::string&& s) {
    text = std::move(s);
    std::vector<GlyphInfo> glyphs = layoutText(text.c_str(), layout);
    spurt(glyphs);
  }
}