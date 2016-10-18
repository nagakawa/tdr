#pragma once

#include <stdint.h>

#include <functional>
#include <vector>

#include <Collidable.h>

namespace tdr {
  typedef std::function<void(Collidable*, Collidable*)> CollisionCallback;
  class CollisionRegistrar;
  struct CollisionPair {
    CollisionCallback cback;
    uint16_t i1, i2;
    friend void perform(CollisionPair& p, CollisionRegistrar& registrar);
  };
  class CollisionRegistrar {
  public:
    template<typename C>
    uint16_t registerCollidable(C* c) {
      uint16_t size = (uint16_t) collidables.size();
      if (size == 0xFFFF) throw "This registrar is full; no more elements can be registered";
      collidables.push_back(static_cast<Collidable*>(c));
      return size;
    }
    template<typename C1, typename C2>
    void registerCallback(uint16_t i1, uint16_t i2, std::function<void(C1*, C2*)> cback) {
      CollisionPair p;
      p.i1 = i1;
      p.i2 = i2;
      p.cback = cback;
      callbacks.push_back(p);
    }
    void performAll();
    friend void perform(CollisionPair& p, CollisionRegistrar& registrar);
  private:
    //std::unordered_multimap<int, Collidable*> collidables;
    std::vector<CollisionPair> callbacks;
    std::vector<Collidable*> collidables;
  };
  void perform(CollisionPair& p, CollisionRegistrar& registrar);
}
