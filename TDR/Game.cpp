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

