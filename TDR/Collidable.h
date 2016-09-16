#pragma once

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <memory>
#include <Renderable.h>
#include "CollisionIterator.h"
#include "fixedpoint.h"
#include "hitbox.h"

namespace tdr {
  class Collidable: public agl::Renderable {
  public:
    // How many objects there are to check collision for.
    virtual int count() = 0;
    // How much time it takes to check collisions against this collection.
    virtual int strength() = 0;
    virtual bool check(const Circle& h) = 0;
    virtual bool check(const Line& h) = 0;
    virtual std::unique_ptr<CollisionIterator> iterator() const = 0;
    virtual int collisionClass() { return -1; }
  };
}
