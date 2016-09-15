#include "hitbox.h"

#include <algorithm>

using namespace tdr;

bool tdr::doCirclesIntersect(const Circle& h1, const Circle& h2) {
  return isWithin(h1.x - h2.x, h1.y - h2.y, h1.radius + h2.radius);
}

// Thanks http://stackoverflow.com/a/1088058
bool tdr::doCircleAndLineIntersect(const Circle& h1, const Line& h2) {
  fix1616 radius = h1.radius + h2.width;
  int32_t dx, dy;
  sincos(h2.angle, dx, dy);
  fix1616 t = multiply1616By230((h1.x - h2.x), dx) + multiply1616By230((h1.y - h2.y), dy);
  // might intersect line, but not segment
  if (t < -1 || t > 1) return false;
  fix1616 ex = multiply1616By230(t, dx) + h2.x;
  fix1616 ey = multiply1616By230(t, dy) + h2.y;
  fix1616 dist = hypotx(h1.x - ex, h1.y - ey);
  return dist < radius;
}

// Because linear hitboxes are actually rectangular, the algorithm for
// checking their intersection is quite complicated.
// Thanks http://www.gamedev.net/page/resources/_/technical/game-programming/2d-rotated-rectangle-collision-r2604
// now if you don't mind, I need to go to sleep
bool tdr::doLinesIntersect(const Line& h1, const Line& h2) {
  fix1616 dist = hypotx(h1.length, h1.width) + hypotx(h2.length, h2.width);
  if (hypotx(h1.x - h2.x, h1.y - h2.y) >= dist) return false;
  int dx1, dy1, dx2, dy2;
  sincos(h1.angle, dx1, dy1);
  sincos(h2.angle, dx2, dy2);
  const fix1616 d1ax = multiply1616By230(h1.length, dx1);
  const fix1616 d1ay = multiply1616By230(h1.length, dy1);
  const fix1616 d1bx = multiply1616By230(-h1.length, dy1);
  const fix1616 d1by = multiply1616By230(h1.length, dx1);
  const fix1616 d2ax = multiply1616By230(h2.length, dx2);
  const fix1616 d2ay = multiply1616By230(h2.length, dy2);
  const fix1616 d2bx = multiply1616By230(-h2.length, dy2);
  const fix1616 d2by = multiply1616By230(h2.length, dx2);
  // yet more preprocessor sorcery
#define MKPROJ(corner, rect, axis) \
    const fix1616 corner##rect##axis##x = h##rect.x + d##axis##x; \
    const fix1616 corner##rect##axis##y = h##rect.y + d##axis##y; \
    const fix1616 proj##corner##axis##rect = \
      (corner##rect##axis##x * d##axis##x + corner##rect##axis##y * d##axis##y) / \
      (corner##rect##axis##x * corner##rect##axis##x + corner##rect##axis##y * corner##rect##axis##y);
#define MKPROJS(rect, axis) \
    MKPROJ(ur, rect, axis); \
    MKPROJ(ul, rect, axis); \
    MKPROJ(lr, rect, axis); \
    MKPROJ(ll, rect, axis); \
    const fix1616 maxProj##axis##rect = max(proj##ur##axis##rect, max(proj##ul##axis##rect, max(proj##lr##axis##rect, proj##ll##axis##rect))); \
    const fix1616 minProj##axis##rect = min(proj##ur##axis##rect, min(proj##ul##axis##rect, min(proj##lr##axis##rect, proj##ll##axis##rect)));
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
