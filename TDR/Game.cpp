#include "Game.h"

#include <functional>

#include <kozet_fixed_point/kfp.h>

using namespace tdr;

tdr::Game::Game(agl::GLFWApplication* app,
  int w, int h, int aw, int ah) :
    p(w, h, aw, ah), gp(kfp::s16_16(w) / 2, kfp::s16_16(h) * 3 / 4),
    bullets(&p, &t), pfSprite(&(p.getTexture())) {
  pfSprite.setApp(app);
  pfSprite.setUp();
  pfSprite.addSprite(agl::Sprite2DInfo{{
    0, 0, (float) aw, (float) ah
  }, {
    0, 0, (float) aw, (float) ah
  }});
}

tdr::Game::~Game() {
}

void tdr::Game::setShotsheet(agl::Texture&& shotsheet) {
  t = std::move(shotsheet);
}

void tdr::Game::update() {
  bullets.updatePositions(
    { 0, 0, (int16_t) p.getWidth(), (int16_t) p.getHeight() });
}

void tdr::Game::render() {
  //p.getFBO().setActive();
  bullets.render();
  agl::setDefaultFBOAsActive();
  pfSprite.tick();
}
