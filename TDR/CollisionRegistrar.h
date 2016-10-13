#pragma once

#include <functional>
#include <vector>

#include <Collidable.h>

namespace tdr {
  typedef std::function<void(Collidable*, Collidable*)> CollisionCallback;
  struct CollisionPair {
    Collidable* c1;
    Collidable* c2;
    CollisionCallback cback;
    void perform() {
      cback(c1, c2);
    }
  };
  class CollisionRegistrar {
  public:
    template<typename C1, typename C2>
    void registerCallback(C1* c1, C2* c2, std::function<void(C1, C2)> cback) {
      CollisionPair p;
      p.c1 = (Collidable) c1;
      p.c2 = (Collidable) c2;
      p.cback = cback;
      callbacks.push_back(p);
    }
    void performAll();
  private:
    //std::unordered_multimap<int, Collidable*> collidables;
    std::vector<CollisionPair> callbacks;
  };
}
