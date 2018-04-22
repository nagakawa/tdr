#include "Player.h"

#include <kozet_fixed_point/kfp.h>

using namespace tdr;

void tdr::PlayerState::evolve() {
  if (flag == ST_HIT && timeLeft == 0) {
    flag = ST_DOWN;
    timeLeft = respawnTime;
  }
  if (flag == ST_DOWN && timeLeft == 0) {
    flag = ST_NORMAL;
    timeLeft = respawnInvincibilityTime;
  }
  if (timeLeft != 0) --timeLeft;
}

tdr::Player::Player(kfp::s16_16 x, kfp::s16_16 y, kfp::s16_16 r) {
  hitbox = { { x, y }, r };
}

void tdr::hit(Player& p, BulletList& bl) {
  PlayerState& st = p.getState();
  st.flag = ST_HIT;
  st.timeLeft = st.deathbombTime;
  (void) bl;
}
