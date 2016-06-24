#include "Collidable.h"

#include <cmath>
#include <algorithm>

using namespace tdr;

bool tdr::doCirclesIntersect(Circle& h1, Circle& h2) {
  return hypot(h1.x - h2.x, h1.y - h2.y) < (h1.radius + h2.radius);
}

// Thanks http://stackoverflow.com/a/1088058
bool tdr::doCircleAndLineIntersect(Circle& h1, Line& h2) {
  float radius = h1.radius + h2.width;
  float dx = cos(h2.angle);
  float dy = sin(h2.angle);
  float t = dx * (h1.x - h2.x) + dy * (h1.y - h2.y);
  // might intersect line, but not segment
  if (t < -1 || t > 1) return false;
  float ex = t * dx + h2.x;
  float ey = t * dy + h2.y;
  float dist = hypot(h1.x - ex, h1.y - ey);
  return dist < radius;
}

bool tdr::doLinesIntersect(Line& h1, Line& h2) {
  float dist = hypot(h1.length, h1.width) + hypot(h2.length, h2.width);
  if (hypot(h1.x - h2.x, h1.y - h2.y) >= dist) return false;
  float d1ax = h1.length * cos(h1.angle);
  float d1ay = h1.length * sin(h1.angle);
  float d1bx = -h1.width * sin(h1.angle);
  float d1by = h1.width * cos(h1.angle);
  float d2ax = h2.length * cos(h2.angle);
  float d2ay = h2.length * sin(h2.angle);
  float d2bx = -h2.width * sin(h2.angle);
  float d2by = h2.width * cos(h2.angle);
#define MKPROJ(corner, rect, axis) \
    float corner##rect##axis##x = h##rect.x + d##axis##x; \
    float corner##rect##axis##y = h##rect.y + d##axis##y; \
    float proj##corner##axis##rect = \
      (corner##rect##axis##x * d##axis##x + corner##rect##axis##y * d##axis##y) / \
      (corner##rect##axis##x * corner##rect##axis##x + corner##rect##axis##y * corner##rect##axis##y);
#define MKPROJS(rect, axis) \
    MKPROJ(ur, rect, axis); \
    MKPROJ(ul, rect, axis); \
    MKPROJ(lr, rect, axis); \
    MKPROJ(ll, rect, axis); \
    float maxProj##axis##rect = std::max(proj##ur##axis##rect, std::max(proj##ul##axis##rect, std::max(proj##lr##axis##rect, proj##ll##axis##rect))); \
    float minProj##axis##rect = std::min(proj##ur##axis##rect, std::min(proj##ul##axis##rect, std::min(proj##lr##axis##rect, proj##ll##axis##rect)));
#define CHKPROJ(axis) \
    MKPROJS(1, axis); \
    MKPROJS(2, axis); \
    if (minProj##axis##1 > maxProj##axis##2 || minProj##axis##2 > maxProj##axis##1) return false;
  CHKPROJ(1a);
  CHKPROJ(1b);
  CHKPROJ(2a);
  CHKPROJ(2b);
  return true;
}
