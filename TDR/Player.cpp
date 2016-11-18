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

std::unique_ptr<CollisionIterator> tdr::Player::iterator() const {
	std::unique_ptr<CollisionIterator> itp(
		new PlayerIterator(*this));
	return itp;
}
