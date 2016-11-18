#include "Player.h"

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

tdr::Player::Player(fix1616 x, fix1616 y, fix1616 r) {
  hitbox = { x, y, r };
}

std::unique_ptr<CollisionIterator> tdr::Player::iterator() const {
	std::unique_ptr<CollisionIterator> itp(
		new PlayerIterator(*this));
	return itp;
}

void tdr::hit(Player& p, BulletList& bl) {
  PlayerState& st = p.getState();
  st.flag = ST_HIT;
  st.timeLeft = st.deathbombTime;
  (void) bl;
}
