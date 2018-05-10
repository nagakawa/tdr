#pragma once

#include <stdint.h>

#include <memory>

#include <kozet_coroutine/kcr.h>

#include <Scene.h>
#include <Sprite2D.h>
#include <Texture.h>

#include "Bullet.h"
#include "Player.h"
#include "Playfield.h"
#include "Replay.h"

namespace tdr {
  class Game : public agl::Scene {
  public:
    Game(
      Shotsheet&& ss,
      agl::Texture&& stgFrame,
      int w, int h,
      int offw = 0, int offh = 0, int aw = 0, int ah = 0);
    ~Game();
    void setShotsheet(Shotsheet&& shotsheet);
    void initialise() override;
    void update() override;
    void render() override;
    BulletList& getBulletList() { return bullets; }
    bool isReplay() const { return reading; }
    virtual void mainLoop() = 0;
  private:
    Playfield p;
    Player gp;
    BulletList bullets;
    agl::Sprite2D pfSprite;
    agl::Texture stgFrame;
    agl::Sprite2D stgFrameSprite;
    Replay r;
    kcr::Manager man;
    bool reading;
  };
}
