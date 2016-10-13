#pragma once

#include <Dimensional.h>
#include <FBO.h>

#include <memory>

namespace tdr {
  class Playfield : public agl::Dimensional {
  public:
    Playfield(int w, int h, int aw = 0, int ah = 0) : w(w), h(h) {
      if (aw == 0) aw = w;
      if (ah == 0) ah = h;
      this->aw = aw;
      this->ah = ah;
      agl::FBOTex ft = agl::makeFBOForMe(aw, ah);
      fbo = ft.fbo;
      t = ft.texture;
    }
    // ~Playfield();
    int getWidth() const { return w; }
    int getHeight() const { return h; }
    int getActualWidth() const { return aw; }
    int getActualHeight() const { return ah; }
    int getFBOID() const { return fbo->id; }
    agl::FBO getFBO() const { return *fbo; }
    agl::Texture getTexture() const { return *t; }
  private:
    int w, h;
    int aw, ah;
    std::shared_ptr<agl::FBO> fbo;
    std::shared_ptr<agl::Texture> t; // Texture associated with FBO
  };
}
