#include "Game.h"

#include <functional>

#include <kozet_fixed_point/kfp.h>

using namespace tdr;

tdr::Game::Game(int w, int h, int aw, int ah) :
    p(w, h, aw, ah), gp(kfp::s16_16(w) / 2, kfp::s16_16(h) * 3 / 4) {}

tdr::Game::~Game() {
}

void tdr::Game::setTexture(std::shared_ptr<agl::Texture> shotsheet) {
  t = shotsheet;
}

void tdr::Game::update() {
  // Nothing here yet
}
