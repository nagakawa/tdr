#pragma once

#include <memory>

#include <Scene.h>

#include "Game.h"

namespace tdr {
  class GameScene : Scene {
  public:
    GameScene(std::unique_ptr<Game>&& g) : g(std::move(g)) {}
    void initialise() override;
    void update() override;
    void render() override;
    // void ~GameScene() override;
    Game& getGame() { return *g; }
  private:
    std::unique_ptr<Game> g;
  };
}
