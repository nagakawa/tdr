#pragma once

#include <stdint.h>

#include <functional>
#include <memory>
#include <vector>

#include <Collidable.h>

namespace tdr {
#if INTPTR_MAX == INT64_MAX
  typedef uint32_t CRHandle;
  #define MAX_CR_ENTRIES 0xFFFFFFFF
#elif INTPTR_MAX == INT32_MAX
  typedef uint16_t CRHandle;
  #define MAX_CR_ENTRIES 0xFFFF
#else
  #error 32- or 64-bit architecture required (16-bit is not supported)
#endif
  typedef std::function<void(Collidable&, Collidable&)> CollisionCallback;
  class CollisionRegistrar;
  struct CollisionPair {
    CollisionCallback cback;
    CRHandle i1, i2;
    friend void perform(CollisionPair& p, CollisionRegistrar& registrar);
  };
  class CollisionRegistrar {
  public:
    template<typename C>
    CRHandle registerCollidable(std::shared_ptr<C> c) {
      CRHandle size = (CRHandle) collidables.size();
      if (size == MAX_CR_ENTRIES) throw "This registrar is full; no more elements can be registered";
      // We use &* here to convert the unique_ptr to a raw pointer, so we can
      // convert it back to a type-erased unique_ptr without having the
      // compiler complain. And yes, we really want to do this.
      collidables.push_back(std::shared_ptr<Collidable>(&*c));
      return size;
    }
    template<typename C1, typename C2>
    void registerCallback(CRHandle i1, CRHandle i2, std::function<void(C1&, C2&)> cback) {
      CollisionPair p;
      p.i1 = i1;
      p.i2 = i2;
      p.cback = static_cast<std::function<void(C1&, C2&)>> (cback);
      callbacks.push_back(p);
    }
    void performAll();
    friend void perform(CollisionPair& p, CollisionRegistrar& registrar);
  private:
    //std::unordered_multimap<int, Collidable*> collidables;
    std::vector<CollisionPair> callbacks;
    std::vector<std::shared_ptr<Collidable>> collidables;
  };
  void perform(CollisionPair& p, CollisionRegistrar& registrar);
}
