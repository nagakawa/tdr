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
    int index() { return i; }
    virtual ~CollisionIterator();
  protected:
    int i = 0;
  };
  template <typename T>
  class SingletonCollisionIterator : public CollisionIterator {
  public:
    SingletonCollisionIterator(T thing) : thing(thing) {}
    bool isEmpty() const { return i == 0; }
    virtual bool isLine() const = 0;
    virtual const Circle& getCircle() const = 0;
    virtual const Line& getLine() const = 0;
    void next() { i = 1; }
  protected:
    const T& contents() const { return thing; }
  private:
    const T thing;
  };
  template <typename T>
  class ArrayCollisionIterator : public CollisionIterator {
  public:
    ArrayCollisionIterator(const T* things, int count) :
      count(count), things(things) {}
    bool isEmpty() const { return i >= count; }
    virtual bool isLine() const = 0;
    virtual const Circle& getCircle() const = 0;
    virtual const Line& getLine() const = 0;
    void next() { ++i; }
  protected:
    const T& contents() const { return things[i]; }
  private:
    int count;
    const T* things;
  };
}
