#pragma once

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <Renderable.h>

namespace tdr {
  struct fix1616 {
    int32_t u;
  	fix1616 operator+(fix1616 b);
  	fix1616 operator-(fix1616 b);
    fix1616 operator-();
    fix1616 operator+=(fix1616 b);
  	fix1616 operator-=(fix1616 b);
  	fix1616 operator*(fix1616 b);
  	fix1616 operator/(fix1616 b);
    fix1616 operator=(int b);
  	bool operator==(fix1616 b);
    bool operator!=(fix1616 b);
  	bool operator<(fix1616 b);
    bool operator>(fix1616 b);
    bool operator>=(fix1616 b);
    bool operator==(int b);
    bool operator!=(int b);
  	bool operator<(int b);
    bool operator>(int b);
  };
	#define CELESTIAL 0x7FFFFFFF
	#define ABYSS ((int32_t) 0x80000000)
	bool isWithin(fix1616 x, fix1616 y, fix1616 r);
  fix1616 hypotx(fix1616 x, fix1616 y);
	fix1616 multiply1616By230(fix1616 a, int32_t b);
	inline fix1616 intToFix(int i) {
		return { i << 16 };
	}
	void sincos(fix1616 t, int32_t& c, int32_t& s);
  void rectp(fix1616 c, fix1616 s, fix1616& r, fix1616& t);
  fix1616 max(fix1616 a, fix1616 b);
  fix1616 min(fix1616 a, fix1616 b);
  fix1616 clamp(fix1616 x, fix1616 a, fix1616 b);
  int32_t sqrti(int64_t n);
	const fix1616 C_ZERO = ((fix1616) { 0 });
	const fix1616 C_ONE = ((fix1616) { 0x10000 });
  struct Circle {
    fix1616 x, y, radius;
  };
  struct Line {
    // Actually a line segment.
    // Scratch that. It's a RECTANGLE.
    // Note: length is actually half the actual length;
    // i. e. the distance from the midpoint to one of the endpoints.
    fix1616 x, y, angle, width, length;
  };
  bool doCirclesIntersect(Circle& h1, Circle& h2);
  bool doCircleAndLineIntersect(Circle& h1, Line& h2);
  bool doLinesIntersect(Line& h1, Line& h2);
  fix1616 clamp(fix1616 x, fix1616 a, fix1616 b);
  static_assert(offsetof(Circle, x) == offsetof(Line, x) &&
    offsetof(Circle, y) == offsetof(Line, y),
    "Offsets of x and y must be the same in Circle and Line");
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
