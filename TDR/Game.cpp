#include "Game.h"

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

CRHandle tdr::Game::registerBulletList(std::shared_ptr<BulletList>& bl) {
  std::shared_ptr<BulletList> entry(new BulletList(p, t));
  bl = entry;
  return registrar->registerCollidable(entry);
}

void tdr::Game::update() {
  registrar->updateAll();
}
