#include "CollisionRegistrar.h"

using namespace tdr;

void tdr::CollisionRegistrar::performAll() {
  for (CollisionPair& callback : callbacks) callback.perform();
}
