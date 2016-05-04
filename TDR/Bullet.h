#pragma once

#include <stdint.h>

namespace tdr {
	typedef struct { int32_t u; } fix1616;
#define F16_ZERO { 0 }
#define F16_ONE { 1 }
#define CELESTIAL 0x7FFFFFFF
#define ABYSS ((int32_t) 0x80000000)
	fix1616 operator+(fix1616 a, fix1616 b);
	fix1616 operator-(fix1616 a, fix1616 b);
	fix1616 operator*(fix1616 a, fix1616 b);
	fix1616 operator/(fix1616 a, fix1616 b);
	bool operator==(fix1616 a, fix1616 b);
	bool operator<(fix1616 a, fix1616 b);
	bool isWithin(fix1616 x, fix1616 y, fix1616 r);
	fix1616 multiply1616By230(fix1616 a, uint32_t b);
	inline fix1616 intToFix(int i) {
		return { i << 16 };
	}
	void sincos(fix1616 t, int32_t& c, int32_t& s);
#ifdef FIXED_POINT
	typedef fix1616 Coord;
#else
	typedef float Coord;
#endif
	struct Bullet {
		uint64_t id;
		Coord x, y;
		Coord xs, ys;
		Coord xa, ya;
		Coord speed, angle, angularVelocity;
		Coord visualAngle;
		// Width and length would be the same for ordinary bullets,
		// but different for lasers.
		Coord visualWidth, collisionWidth;
		Coord visualLength, collisionLength;
		uint16_t left, top, right, bottom; // texcoords
		// Hopefully no one wants to graze anything less often than
		// 128 frames.
		//
		// n > 0 -> every n frames
		// 0 -> only once
		// -1 -> never
		int8_t grazeFrequency;
		// n > 0 -> in n frames
		// 0 -> ready
		// -1 -> can't graze anymore
		int8_t timeToNextGraze;
		int isLaser : 1;
		int markedForDeletion : 1;
		int useRadial : 1;
		int detachVisualAndMovementAngles : 1;
	};
}
