#pragma once

#include <stdint.h>

#include <memory>

#include <Texture.h>

#include "Bullet.h"
#include "Player.h"
#include "Playfield.h"

namespace tdr {
  class Game {
  public:
    Game(int w, int h, int aw = 0, int ah = 0);
    ~Game();
    void setTexture(agl::Texture&& shotsheet);
    void update();
    void render();
  private:
    Playfield p;
    agl::Texture t;
    Player gp;
    BulletList bullets;
  };
}
