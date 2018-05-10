#include "Game.h"

#include <functional>

#include <kozet_fixed_point/kfp.h>

#include <ScenedGLFWApplication.h>

using namespace tdr;

tdr::Game::Game(
  Shotsheet&& ss,
  agl::Texture&& stgFrame,
  int w, int h, int offw, int offh, int aw, int ah) :
    p(w, h, offw, offh, aw, ah),
    gp(kfp::s16_16(w) / 2, kfp::s16_16(h) * 3 / 4),
    bullets(&p, std::move(ss)),
    pfSprite(&(p.getTexture())),
    stgFrame(std::move(stgFrame)),
    stgFrameSprite(&this->stgFrame) {
  pfSprite.addSprite(agl::Sprite2DInfo{
    p.getActualBoundsZero(),
    p.getActualBounds(),
  });
  bullets.setUp();
  man.setAdvanceOnExit(false);
  man.spawn([this]() {
    while (true) {
      mainLoop();
      kcr::yield();
      kcr::exit();
    }
  });
}

tdr::Game::~Game() {
}

void tdr::Game::initialise() {
  // nothing yet
  pfSprite.setApp(app);
  pfSprite.setUp();
  stgFrameSprite.addSprite(agl::Sprite2DInfo{
    { 0, 0, (float) app->getWidth(), (float) app->getHeight() },
    { 0, 0, (float) app->getWidth(), (float) app->getHeight() }
  });
  stgFrameSprite.setApp(app);
  stgFrameSprite.setUp();
}

void tdr::Game::setShotsheet(Shotsheet&& shotsheet) {
  bullets.shotsheet = std::move(shotsheet);
}

void tdr::Game::update() {
  man.enter();
  bullets.updatePositions(
    { 0, 0, (int16_t) p.getWidth(), (int16_t) p.getHeight() });
}

void tdr::Game::render() {
  //p.getFBO().setActive();
  glClearColor(0.5f, 0.7f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  bullets.render();
  p.blit();
  agl::setDefaultFBOAsActive();
  pfSprite.tick();
  stgFrameSprite.tick();
}
