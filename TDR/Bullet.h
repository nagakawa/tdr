#pragma once

#include <stdint.h>

#include <rect.h>
#include "Collidable.h"

namespace tdr {
	struct Graphic {
		agl::UIRect16 texcoords;
		fix1616 visualRadius;
		fix1616 collisionRadius;
	};
	class Bullet {
	public:
		uint64_t id;
		union {
			Circle c;
			Line l;
		} hitbox;
		fix1616 xs, ys;
		fix1616 xa, ya;
		fix1616 speed, angle, angularVelocity;
		fix1616 visualAngle;
		// Width and length would be the same for ordinary bullets,
		// but different for lasers.
		fix1616 visualWidth, visualLength;
		agl::UIRect16 texcoords;
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
		uint8_t delay;
		uint8_t isLaser;
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
		// Note: none of the constructors register the bullet.
		Bullet(fix1616 x, fix1616 y, fix1616 speed, fix1616 angle, Graphic& graph, uint8_t delay); // CreateShotA1
	};
	class BulletList: public Collidable {
		// TODO
	};
}
