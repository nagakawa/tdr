#include "Bullet.h"

#include <math.h>

using namespace tdr;

void tdr::Bullet::update() {
	if (useRadial) {
		xs = speed * cos(angle);
		ys = speed * sin(angle);
	} else {
		speed = hypot(xs, ys); // Glad we have this!
		angle = atan2(ys, xs);
	}
	x += xs;
	y += ys;
	xs += xa;
	ys += ya;
	if (!detachVisualAndMovementAngles) visualAngle = angle;
}

bool tdr::Bullet::graze() {
	if (timeToNextGraze != 0 || grazeFrequency == -1) return false;
	timeToNextGraze = grazeFrequency == 0 ? -1 : grazeFrequency - 1;
	return true;
}

void tdr::Bullet::refreshGraze() {
	if (timeToNextGraze != 0 && grazeFrequency != -1) --timeToNextGraze;
}
