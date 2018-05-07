#pragma once

#include <Dimensional.h>
#include <FBO.h>

#include <memory>

namespace tdr {
  class Playfield : public agl::Dimensional {
  public:
    Playfield(int w, int h, int offw, int offh, int aw = 0, int ah = 0) :
        w(w), h(h), aw((aw == 0) ? w : aw), ah((ah == 0) ? h : ah),
        offw(offw), offh(offh) {
      agl::FBOTexMS ft = agl::makeFBOForMeMS(this->aw, this->ah);
      fbo = std::move(ft.ss.fbo);
      tex = std::move(ft.ss.texture);
      fboMS = std::move(ft.ms.fbo);
      texMS = std::move(ft.ms.texture);
      aoffw = offw * w / this->aw;
      aoffh = offh * h / this->ah;
    }
    void blit() {
      fboMS.blitTo(fbo, aw, ah);
    }
    // ~Playfield();
    int getWidth() const { return w; }
    int getHeight() const { return h; }
    int getActualWidth() const { return aw; }
    int getActualHeight() const { return ah; }
    int getOffsetX() const { return offw; }
    int getOffsetY() const { return offh; }
    int getActualOffsetX() const { return aoffw; }
    int getActualOffsetY() const { return aoffh; }
    int getFBOID() const { return fbo.id; }
    agl::Rect getActualBounds() const {
      return {
        (float) aoffw, (float) aoffh,
        (float) (aoffw + aw), (float) (aoffh + ah)
      };
    }
    agl::Rect getActualBoundsZero() const {
      return {
        0, 0,
        (float) aw, (float) ah
      };
    }
    agl::FBO& getFBO() { return fbo; }
    agl::FBO& getFBOMS() { return fboMS; }
    agl::Texture& getTexture() { return tex; }
    const agl::FBO& getFBO() const { return fbo; }
    const agl::Texture& getTexture() const { return tex; }
  private:
    int w, h;
    int aw, ah;
    int offw, offh;
    int aoffw, aoffh;
    agl::FBO fbo;
    agl::Texture tex; // Texture associated with FBO
    agl::FBO fboMS;
    agl::Texture texMS; // Texture associated with FBO
  };
}
