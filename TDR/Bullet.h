#pragma once

#include <stdint.h>

namespace tdr {
	typedef struct { int32_t u; } fix1616;
	fix1616 operator+(fix1616 a, fix1616 b);
	fix1616 operator-(fix1616 a, fix1616 b);
	fix1616 operator*(fix1616 a, fix1616 b);
	fix1616 operator/(fix1616 a, fix1616 b);
	struct Bullet {
		fix1616 x, y;
		fix1616 xs, ys;
		fix1616 xa, ya;
		fix1616 speed, angle, angularVelocity;
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
		int radialAndRectangularDetached : 1;
	};
}
