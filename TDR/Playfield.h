#pragma once

#include <FBO.h>

#include <memory>

namespace tdr {
  class Playfield {
  public:
    Playfield(int w, int h) : w(w), h(h) {
      agl::FBOTex ft = agl::makeFBOForMe(w, h);
      fbo = ft.fbo;
      t = ft.texture;
    }
    // ~Playfield();
    int width() const { return w; }
    int height() const { return h; }
    agl::Texture getTexture() const { return *t; }
  private:
    int w, h;
    std::shared_ptr<agl::FBO> fbo;
    std::shared_ptr<agl::Texture> t;
  };
}
