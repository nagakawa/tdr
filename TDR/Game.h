#pragma once

#include <stdint.h>

#include <memory>

#include <Sprite2D.h>
#include <Texture.h>

#include "Bullet.h"
#include "Player.h"
#include "Playfield.h"
#include "Replay.h"

namespace tdr {
  class Game {
  public:
    Game(agl::GLFWApplication* app,
      int w, int h, int aw = 0, int ah = 0);
    ~Game();
    void setShotsheet(agl::Texture&& shotsheet);
    void update();
    void render();
    bool isReplay() const { return reading; }
  private:
    Playfield p;
    agl::Texture t;
    Player gp;
    BulletList bullets;
    agl::Sprite2D pfSprite;
    Replay r;
    bool reading;
  };
}
