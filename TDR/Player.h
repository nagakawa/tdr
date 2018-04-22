#pragma once

#include <memory>

#include <kozet_fixed_point/kfp.h>

#include "Bullet.h"

using namespace kfp::literals;

namespace tdr {
  typedef enum { ST_NORMAL, ST_HIT, ST_DOWN, ST_END } PlayerStateFlag;
  struct PlayerState {
    PlayerStateFlag flag;
    // ST_NORMAL - frames left of invincibility
    // ST_HIT - frames left before you die
    // ST_DOWN - frames left before you respawn
    int timeLeft;
    int deathbombTime;
    int respawnTime;
    int respawnInvincibilityTime;
    PlayerState() : flag(ST_NORMAL), timeLeft(0),
        deathbombTime(15), respawnTime(60), respawnInvincibilityTime(120) {}
    void evolve();
  };
  class Player {
  public:
    Player(
      kfp::s16_16 x, kfp::s16_16 y,
      kfp::s16_16 r = "1.5"_s16_16);
    bool check(const Circle& h) { return h.intersects(hitbox); }
    bool check(const Line& h) { return h.intersects(hitbox); }
    const Circle& getHitbox() const { return hitbox; }
    PlayerState& getState() { return state; }
  private:
    Circle hitbox;
    PlayerState state;
  };
  void hit(Player& p, BulletList& bl);
}
