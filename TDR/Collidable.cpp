#include "Collidable.h"

#include <cmath>
#include <algorithm>

using namespace tdr;

bool tdr::doCirclesIntersect(Circle& h1, Circle& h2) {
  return hypotx(h1.x - h2.x, h1.y - h2.y) < (h1.radius + h2.radius);
}

// Thanks http://stackoverflow.com/a/1088058
bool tdr::doCircleAndLineIntersect(Circle& h1, Line& h2) {
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
bool tdr::doLinesIntersect(Line& h1, Line& h2) {
  fix1616 dist = hypotx(h1.length, h1.width) + hypotx(h2.length, h2.width);
  if (hypotx(h1.x - h2.x, h1.y - h2.y) >= dist) return false;
  int dx1, dy1, dx2, dy2;
  sincos(h1.angle, dx1, dy1);
  sincos(h2.angle, dx2, dy2);
  fix1616 d1ax = multiply1616By230(h1.length, dx1);
  fix1616 d1ay = multiply1616By230(h1.length, dy1);
  fix1616 d1bx = multiply1616By230(-h1.length, dy1);
  fix1616 d1by = multiply1616By230(h1.length, dx1);
  fix1616 d2ax = multiply1616By230(h2.length, dx2);
  fix1616 d2ay = multiply1616By230(h2.length, dy2);
  fix1616 d2bx = multiply1616By230(-h2.length, dy2);
  fix1616 d2by = multiply1616By230(h2.length, dx2);
  // yet more preprocessor sorcery
#define MKPROJ(corner, rect, axis) \
    fix1616 corner##rect##axis##x = h##rect.x + d##axis##x; \
    fix1616 corner##rect##axis##y = h##rect.y + d##axis##y; \
    fix1616 proj##corner##axis##rect = \
      (corner##rect##axis##x * d##axis##x + corner##rect##axis##y * d##axis##y) / \
      (corner##rect##axis##x * corner##rect##axis##x + corner##rect##axis##y * corner##rect##axis##y);
#define MKPROJS(rect, axis) \
    MKPROJ(ur, rect, axis); \
    MKPROJ(ul, rect, axis); \
    MKPROJ(lr, rect, axis); \
    MKPROJ(ll, rect, axis); \
    fix1616 maxProj##axis##rect = max(proj##ur##axis##rect, max(proj##ul##axis##rect, max(proj##lr##axis##rect, proj##ll##axis##rect))); \
    fix1616 minProj##axis##rect = min(proj##ur##axis##rect, min(proj##ul##axis##rect, min(proj##lr##axis##rect, proj##ll##axis##rect)));
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

fix1616 tdr::clamp(fix1616 x, fix1616 a, fix1616 b) {
  return max(a, min(b, x));
}

fix1616 tdr::fix1616::operator+(fix1616 b) {
	if (b.u > 0 && ((int32_t) CELESTIAL) - b.u < u)
		return { (int32_t) CELESTIAL };
	if (b.u < 0 && ((int32_t) ABYSS) - b.u > u)
		return { ((int32_t) ABYSS) };
	return { u + b.u };
}
fix1616 tdr::fix1616::operator-(fix1616 b) {
	if (b.u > 0 && -((int32_t) CELESTIAL) + b.u < u)
		return { -(int32_t) CELESTIAL };
	if (b.u < 0 && ((int32_t) ABYSS) + b.u > u)
		return { ((int32_t) ABYSS) };
	return { u - b.u };
}
fix1616 tdr::fix1616::operator-() {
	if (u == ABYSS) return { CELESTIAL };
  else return { -u };
}
fix1616 tdr::fix1616::operator*(fix1616 b) {
	int64_t prod = (((int64_t) u) * b.u) >> 16;
	if (prod >= 0x100000000) prod = CELESTIAL;
	if (prod < -((int64_t) 0x100000000)) prod = ((int64_t) ABYSS);
	return { (int32_t) prod };
}
fix1616 tdr::fix1616::operator/(fix1616 b) {
	int64_t aex = (((int64_t) u) << 16);
	int64_t res = aex / b.u;
	if (res > CELESTIAL) return { CELESTIAL };
	if (res < ABYSS) return { ABYSS };
	return { (int32_t) res };
}

bool tdr::fix1616::operator==(fix1616 b) {
	return u == b.u;
}

bool tdr::fix1616::operator!=(fix1616 b) {
	return u != b.u;
}

bool tdr::fix1616::operator<(fix1616 b) {
	return u < b.u;
}

bool tdr::fix1616::operator>(fix1616 b) {
	return u > b.u;
}

bool tdr::fix1616::operator>=(fix1616 b) {
	return u >= b.u;
}

bool tdr::fix1616::operator==(int b) {
	if (b < -0x8000 || b >= 0x8000) return false;
  return (b << 16) == u;
}

bool tdr::fix1616::operator!=(int b) {
	if (b < -0x8000 || b >= 0x8000) return true;
  return (b << 16) != u;
}

bool tdr::fix1616::operator<(int b) {
  if (b < -0x8000) return false;
  if (b >= 0x8000) return true;
  return (b << 16) < u;
}

bool tdr::fix1616::operator>(int b) {
  if (b < -0x8000) return true;
  if (b >= 0x8000) return false;
  return (b << 16) > u;
}

fix1616 tdr::fix1616::operator+=(fix1616 b) {
  u = ((*this) + b).u;
  return *this;
}

fix1616 tdr::fix1616::operator-=(fix1616 b) {
  u = ((*this) - b).u;
  return *this;
}

fix1616 tdr::fix1616::operator=(int b) {
  if (b >= 0x8000) u = CELESTIAL;
  else if (b < -0x8000) u = ABYSS;
  else u = b << 16;
  return *this;
}

fix1616 tdr::max(fix1616 a, fix1616 b) {
  return (a > b) ? a : b;
}

fix1616 tdr::min(fix1616 a, fix1616 b) {
  return (a < b) ? a : b;
}

// Adapted from:
// http://www.codecodex.com/wiki/Calculate_an_integer_square_root
int32_t tdr::sqrti(int64_t n) {
  if (n < 0) throw "Positive n expected in tdr::sqrti";
  int64_t root = 0;
  int64_t rem = n;
  int64_t place = 0x4000'0000'0000'0000; //' // < this compiles, but
  while (place > rem) place >>= 2; // the commented quote was inserted
  while (place != 0) {             // to avoid throwing C++14-unaware
    if (rem >= root + place) {     // syntax highlighters off
      rem -= root + place;
      root += place << 1;
    }
    root >>= 1;
    place >>= 2;
  }
  return (int32_t) root;
}

bool tdr::isWithin(fix1616 x, fix1616 y, fix1616 r) {
	uint64_t distsq = ((int64_t) x.u) * x.u + ((int64_t) y.u) * y.u;
	uint64_t thresh = ((int64_t) r.u) * r.u;
	return distsq <= thresh;
}

fix1616 tdr::hypotx(fix1616 x, fix1616 y) {
  uint64_t distsq = ((int64_t) x.u) * x.u + ((int64_t) y.u) * y.u;
  return { sqrti(distsq) };
}

fix1616 tdr::multiply1616By230(fix1616 a, int32_t b) {
	int64_t res = ((int64_t) a.u) * b >> 30;
	return { (int32_t) res };
}

#define TAU_TIMES_2TT28 0x6487ED51
#define CORDIC_ITERATIONS 31
// CORDIC K constant in 2.30 format.
#define CORDIC_K 0x26DD3B6A

inline int32_t mul230(int32_t a, int32_t b) {
  int64_t aa = a;
  aa *= b;
  return (int32_t) (aa >> 30);
}


// Generated by this Perl6 expression:
// ((^32).map({"0x"~(2**30*atan(2**-$_)).floor.base(16)})).join(", ")
const int32_t arctangents[] = {
  0x3243F6A8, 0x1DAC6705, 0xFADBAFC, 0x7F56EA6,
  0x3FEAB76, 0x1FFD55B, 0xFFFAAA, 0x7FFF55,
  0x3FFFEA, 0x1FFFFD, 0xFFFFF, 0x7FFFF,
  0x3FFFF, 0x1FFFF, 0xFFFF, 0x7FFF,
  0x3FFF, 0x1FFF, 0xFFF, 0x7FF,
  0x3FF, 0x1FF, 0xFF, 0x7F,
  0x3F, 0x1F, 0xF, 0x8,
  0x4, 0x2, 0x1, 0x0
};

// Calculates sine and cosine using CORDIC
// (https://en.wikipedia.org/wiki/CORDIC)
// t = angle in 1/65536 of a turn
// c = reference to where you want cosine to be stored
// s = reference to where you want sine to be stored
// Both of the results will be stored in 2.30 format.
void tdr::sincos(fix1616 t, int32_t& c, int32_t& s) {
	bool inv = t.u > 0x40000000 || t.u < -((int32_t) 0x40000000);
	if (inv) // Plus, minus, that doesn't even matter ~
		t.u = (int32_t) (((uint32_t) t.u) + 0x80000000); // avoid UB
	// Multiply by tau to convert turns into radians (in 34.30 format)
  // No idea why we need to shift right by THIRTY, though.
	int64_t radians = ((((int64_t) t.u) * TAU_TIMES_2TT28) >> 30);
	// vx and vy use 2.30 format
	// (to be able to represent both 1 and -1)
	int32_t vx = CORDIC_K;
	int32_t vy = 0;
	for (int i = 0; i < CORDIC_ITERATIONS; ++i) {
		// new vector = [1, -factor; factor, 1] old vector
    /*printf("Iteration %d: angle = %f; vector: (%f, %f)\n",
      i, (double) radians / (1 << 30),
      (double) vx / (1 << 30), (double) vy / (1 << 30)
    );*/
    int32_t nx, ny;
		if (radians >= 0) {
      nx = vx - (vy >> i);
      ny = (vx >> i) + vy;
    } else {
      nx = vx + (vy >> i);
      ny = -(vx >> i) + vy;
    }
		vx = nx;
		vy = ny;
		radians += (radians < 0) ? arctangents[i] : -arctangents[i];
	}
	c = inv ? -vx : vx;
	s = inv ? -vy : vy;
}

// Calculating atan2 using CORDIC
void tdr::rectp(fix1616 c, fix1616 s, fix1616& r, fix1616& t) {
  if (c < C_ZERO) {
    rectp(-c, -s, r, t);
    t.u += ABYSS;
    return;
  }
  int64_t angle = 0;
	// vx and vy use 16.16 format
  int32_t vx = c.u;
  int32_t vy = s.u;
  for (int i = 0; i < CORDIC_ITERATIONS; ++i) {
    int32_t nx, ny;
    if (vy <= 0) {
      nx = vx - (vy >> i);
      ny = (vx >> i) + vy;
    } else {
      nx = vx + (vy >> i);
      ny = -(vx >> i) + vy;
    }
		vx = nx;
    angle += (vy > 0) ? arctangents[i] : -arctangents[i];
		vy = ny;
  }
  t.u = (angle << 30) / TAU_TIMES_2TT28;
  r.u = (((int64_t) vx) * CORDIC_K) >> 30;
}
