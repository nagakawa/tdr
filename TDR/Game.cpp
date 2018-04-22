#include "Game.h"

#include <functional>

using namespace tdr;

tdr::Game::Game(int w, int h, int aw, int ah) {
  p = std::make_shared<Playfield>(Playfield(w, h, aw, ah));
}

tdr::Game::~Game() {
}

void tdr::Game::setTexture(std::shared_ptr<agl::Texture> shotsheet) {
  t = shotsheet;
}

void tdr::Game::update() {
  // Nothing here yet
}
