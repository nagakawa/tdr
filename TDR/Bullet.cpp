#include "Bullet.h"

#include <math.h>

using namespace tdr;

void tdr::Bullet::update() {
	if (useRadial) {
		int32_t nxs, nxy;
		sincos(angle, nxs, nxy);
		xs = multiply1616By230(speed, nxs);
		ys = multiply1616By230(speed, nxs);
	} else {
		rectp(xs, ys, speed, angle);
	}
	hitbox.c.x += xs;
	hitbox.c.y += ys;
	xs += xa;
	ys += ya;
	if (delay != 0) --delay;
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

tdr::Bullet::Bullet(fix1616 x, fix1616 y, fix1616 speed, fix1616 angle, Graphic& graph, uint8_t delay) {
	this->hitbox.c = (Circle) {x, y, graph.collisionRadius};
	xs = 0;
	ys = 0;
	xa = 0;
	ya = 0;
	isLaser = 0;
	this->speed = speed;
	this->angle = angle;
	visualWidth = visualLength = graph.visualRadius;
	this->delay = delay;
}
