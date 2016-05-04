#pragma once

#include <stdint.h>

namespace tdr {
	typedef struct { int32_t u; } fix1616;
#define F16_ZERO { 0 }
#define F16_ONE { 1 }
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
	struct Bullet {
		uint64_t id;
		fix1616 x, y;
		fix1616 xs, ys;
		fix1616 xa, ya;
		// Angles are represented as such:
		// 0x0000.0000: 0 rad
		// 0x4000.0000: pi/2 rad
		// 0x8000.0000: pi rad
		// 0xC000.0000: 3pi/2 rad
		fix1616 speed, angle, angularVelocity;
		fix1616 visualAngle;
		// Width and length would be the same for ordinary bullets,
		// but different for lasers.
		fix1616 visualWidth, collisionWidth;
		fix1616 visualLength, collisionLength;
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
