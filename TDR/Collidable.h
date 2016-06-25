#pragma once

#include <Renderable.h>

namespace tdr {
  struct Circle {
    float x, y, radius;
  };
  struct Line {
    // Actually a line segment.
    // Scratch that. It's a RECTANGLE.
    // Note: length is actually half the actual length;
    // i. e. the distance from the midpoint to one of the endpoints.
    float x, y, angle, width, length;
  };
  bool doCirclesIntersect(Circle& h1, Circle& h2);
  bool doCircleAndLineIntersect(Circle& h1, Line& h2);
  bool doLinesIntersect(Line& h1, Line& h2);
  float clamp(float x, float a, float b);
  class Collidable: public agl::Renderable {
  public:
    // How many objects there are to check collision for.
    virtual int count() = 0;
    // How much time it takes to check collisions against this collection.
    virtual int strength() = 0;
    virtual bool check(Circle& h) = 0;
    virtual bool check(Line& h) = 0;
    virtual int collisionClass() { return -1; }
  };
}
