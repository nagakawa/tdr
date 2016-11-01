#pragma once

#include <stddef.h>
#include "fixedpoint.h"

namespace tdr {
  struct Circle {
    fix1616 x, y, radius;
  };
  struct Line {
    // Actually a line segment.
    // Scratch that. It's a RECTANGLE.
    // Note: length is actually half the actual length;
    // i. e. the distance from the midpoint to one of the endpoints.
    // (x, y) is the midpoint of the line segment.
    fix1616 x, y, angle, width, length;
  };
  bool doCirclesIntersect(const Circle& h1, const Circle& h2);
  bool doCircleAndLineIntersect(const Circle& h1, const Line& h2);
  bool doLinesIntersect(const Line& h1, const Line& h2);
  fix1616 clamp(fix1616 x, fix1616 a, fix1616 b);
  static_assert(offsetof(Circle, x) == offsetof(Line, x) &&
    offsetof(Circle, y) == offsetof(Line, y),
    "Offsets of x and y must be the same in Circle and Line");
}
