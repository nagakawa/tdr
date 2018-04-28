#include "Game.h"

#include <functional>

#include <kozet_fixed_point/kfp.h>

using namespace tdr;

tdr::Game::Game(int w, int h, int aw, int ah) :
    p(w, h, aw, ah), gp(kfp::s16_16(w) / 2, kfp::s16_16(h) * 3 / 4),
    bullets(&p, &t) {}

tdr::Game::~Game() {
}

void tdr::Game::setTexture(agl::Texture&& shotsheet) {
  t = std::move(shotsheet);
}

void tdr::Game::update() {
  // Nothing here yet
}

void tdr::Game::render() {
  p.getFBO().setActive();
  bullets.render();
}
