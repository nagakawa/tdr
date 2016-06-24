#pragma once

#include <stdint.h>

namespace tdr {
	struct Bullet {
		uint64_t id;
		float x, y;
		float xs, ys;
		float xa, ya;
		float speed, angle, angularVelocity;
		float visualAngle;
		// Width and length would be the same for ordinary bullets,
		// but different for lasers.
		float visualWidth, collisionWidth;
		float visualLength, collisionLength;
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
		uint8_t alpha;
		int isLaser : 1;
		int markedForDeletion : 1;
		// If true, this bullet will base xs and ys from speed and angle.
		// Otherwise, speed and angle depend on xs and ys.
		int useRadial : 1;
		int detachVisualAndMovementAngles : 1;
		// Simulates movement of the bullet.
		void update();
		// Self explanatory. Returns true if graze succeeded, or false if
		// graze failed. Uses traditional Touhou-style graze system.
		bool graze();
		// Advances to next frame in terms of graze.
		// This often means "decrease time to next graze by 1 frame if bullet is
		// not yet grazeable but will be in the future".
		void refreshGraze();
	};
}
