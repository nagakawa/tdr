#pragma once

#include <memory>

#include "Bullet.h"
#include "Collidable.h"
#include "CollisionIterator.h"
#include "fixedpoint.h"
#include "hitbox.h"

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
  class Player : public Collidable {
  public:
    Player(fix1616 x, fix1616 y, fix1616 r = { 0x18000 });
    std::unique_ptr<CollisionIterator> iterator() const;
    // How many objects there are to check collision for.
    int count() { return 1; }
    // How much time it takes to check collisions against this collection.
    int strength() { return 1; }
    bool check(const Circle& h) { return doCirclesIntersect(h, hitbox); }
    bool check(const Line& h) { return doCircleAndLineIntersect(hitbox, h); }
    const Circle& getHitbox() const { return hitbox; }
    PlayerState& getState() { return state; }
  private:
    Circle hitbox;
    PlayerState state;
  };
  class PlayerIterator : public SingletonCollisionIterator<const Player&> {
	public:
		PlayerIterator(const Player& p) :
      SingletonCollisionIterator(p) {}
    bool isLine() const { return false; }
    const Circle& getCircle() const { return contents().getHitbox(); }
    const Line& getLine() const { return *(Line*) &(contents().getHitbox()); }
	};
  void hit(Player& p, BulletList& bl);
}
