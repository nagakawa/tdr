#include "Game.h"

#include <functional>

using namespace tdr;

tdr::Game::Game(int w, int h, int aw, int ah) {
  p = std::make_shared<Playfield>(Playfield(w, h, aw, ah));
  registrar = std::make_unique<CollisionRegistrar>(CollisionRegistrar());
}

tdr::Game::~Game() {
}

void tdr::Game::setTexture(std::shared_ptr<agl::Texture> shotsheet) {
  t = shotsheet;
}

void tdr::Game::registerPlayer(std::shared_ptr<Player> gp) {
  this->gp = gp;
  gph = registrar->registerCollidable(gp);
}

CRHandle tdr::Game::registerBulletList(std::shared_ptr<BulletList>& bl) {
  if (gph == MAX_CR_ENTRIES) throw "Call registerPlayer first";
  std::shared_ptr<BulletList> entry(new BulletList(p, t));
  bl = entry;
  CRHandle h = registrar->registerCollidable(entry);
  registrar->registerCallback(gph, h,
    std::function<void(Player&, BulletList&)>(hit));
  return h;
}

void tdr::Game::update() {
  registrar->updateAll();
}
