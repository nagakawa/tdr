#pragma once

#include <unordered_map>
#include <unordered_set>

#include <Collidable.h>

namespace tdr {
  class CollisionRegistrar {
  public:
  private:
    std::unordered_multimap<int, Collidable*> collidables;
    // std::unordered_set<
  };
}
