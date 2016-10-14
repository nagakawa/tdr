#pragma once

#include <stdint.h>

namespace tdr {
  struct fix1616 {
    int32_t u;
  	fix1616 operator+(fix1616 b) const;
  	fix1616 operator-(fix1616 b) const;
    fix1616 addWrap(fix1616 b) const;
  	fix1616 subWrap(fix1616 b) const;
    fix1616 operator-() const;
    fix1616 operator+=(fix1616 b);
  	fix1616 operator-=(fix1616 b);
  	fix1616 operator*(fix1616 b) const;
  	fix1616 operator/(fix1616 b) const;
    fix1616 operator=(int b);
  	bool operator==(fix1616 b) const;
    bool operator!=(fix1616 b) const;
  	bool operator<(fix1616 b) const;
    bool operator>(fix1616 b) const;
    bool operator>=(fix1616 b) const;
    bool operator==(int b) const;
    bool operator!=(int b) const;
  	bool operator<(int b) const;
    bool operator>(int b) const;
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
	const fix1616 C_ZERO = { 0 };
	const fix1616 C_ONE = { 0x10000 };
}
