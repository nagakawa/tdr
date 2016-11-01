#include "CollisionRegistrar.h"

using namespace tdr;

void tdr::CollisionRegistrar::performAll() {
  for (CollisionPair& callback : callbacks) perform(callback, *this);
}

void tdr::perform(CollisionPair& p, CollisionRegistrar& registrar) {
  p.cback(*registrar.collidables[p.i1], *registrar.collidables[p.i2]);
}

void tdr::CollisionRegistrar::updateAll() {
  for (std::shared_ptr<Collidable> c : collidables) c->update();
}
