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
      agl::FBOTexMS ft = agl::makeFBOForMeMS(aw, ah);
      fbo = std::move(ft.ss.fbo);
      tex = std::move(ft.ss.texture);
      fboMS = std::move(ft.ms.fbo);
      texMS = std::move(ft.ms.texture);
    }
    // ~Playfield();
    int getWidth() const { return w; }
    int getHeight() const { return h; }
    int getActualWidth() const { return aw; }
    int getActualHeight() const { return ah; }
    int getFBOID() const { return fbo.id; }
    agl::FBO& getFBO() { return fbo; }
    agl::Texture& getTexture() { return tex; }
    const agl::FBO& getFBO() const { return fbo; }
    const agl::Texture& getTexture() const { return tex; }
  private:
    int w, h;
    int aw, ah;
    agl::FBO fbo;
    agl::Texture tex; // Texture associated with FBO
    agl::FBO fboMS;
    agl::Texture texMS; // Texture associated with FBO
  };
}
