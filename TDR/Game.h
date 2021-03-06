#pragma once

#include <stdint.h>

#include <memory>

#include <Texture.h>

#include <Bullet.h>
#include "CollisionRegistrar.h"
#include "Player.h"
#include "Playfield.h"

namespace tdr {
  class Game {
  public:
    Game(int w, int h, int aw = 0, int ah = 0);
    ~Game();
    void setTexture(std::shared_ptr<agl::Texture> shotsheet);
    void registerPlayer(std::shared_ptr<Player> gp);
    CRHandle registerBulletList(std::shared_ptr<BulletList>& bl);
    void update();
  private:
    std::shared_ptr<Playfield> p;
    std::unique_ptr<CollisionRegistrar> registrar;
    std::shared_ptr<agl::Texture> t;
    std::shared_ptr<Player> gp;
    CRHandle gph = MAX_CR_ENTRIES;
  };
}
