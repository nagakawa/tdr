#pragma once

#include "Collidable.h"
#include "fixedpoint.h"
#include "hitbox.h"

namespace tdr {
  class CollisionIterator {
  public:
    virtual bool isEmpty() const = 0;
    virtual bool isLine() const = 0;
    virtual const Circle& getCircle() const = 0;
    virtual const Line& getLine() const = 0;
    virtual void next();
  };
  template <typename T>
  class SingletonCollisionIterator : public CollisionIterator {
  public:
    SingletonCollisionIterator(T thing) : thing(thing) {}
    bool isEmpty() const { return !past; }
    virtual bool isLine() const = 0;
    virtual const Circle& getCircle() const = 0;
    virtual const Line& getLine() const = 0;
    void next() { past = true; }
  protected:
    const T& contents() const { return thing; }
  private:
    bool past = false;
    const T thing;
  };
  template <typename T>
  class ArrayCollisionIterator : public CollisionIterator {
  public:
    ArrayCollisionIterator(const T* things, int count) :
      count(count), things(things) {}
    bool isEmpty() const { return index >= count; }
    virtual bool isLine() const = 0;
    virtual const Circle& getCircle() const = 0;
    virtual const Line& getLine() const = 0;
    void next() { ++index; }
  protected:
    const T& contents() const { return things[count]; }
  private:
    int index = 0;
    int count;
    const T* things;
  };
}
