#pragma once

#include <assert.h>
#include <stdint.h>
#include <vector>

#include <BlendMode.h>
#include <FBO.h>
#include <GLFWApplication.h>
#include <rect.h>
#include <Renderable.h>
#include <Shader.h>
#include <ShaderProgram.h>
#include <VAO.h>
#include <VBO.h>
#include "Collidable.h"
#include "Playfield.h"

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
		int deleteWhenOutOfBounds : 1;
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
		Bullet(
			fix1616 x, fix1616 y,
			fix1616 speed, fix1616 angle,
			Graphic& graph, uint8_t delay) : // CreateShotA1
			xs(C_ZERO), ys(C_ZERO), xa(C_ZERO), ya(C_ZERO),
			speed(speed), angle(angle), angularVelocity(C_ZERO),
			visualWidth(graph.visualRadius), visualLength(graph.visualRadius),
			texcoords(graph.texcoords), delay(delay), isLaser(0),
			markedForDeletion(0), useRadial(1), detachVisualAndMovementAngles(0),
			deleteWhenOutOfBounds(1) {}
	};
	static_assert(offsetof(Bullet, visualAngle) + 4 == offsetof(Bullet, visualWidth),
		"Offset of visualWidth in Bullet must be exactly 4 more than that of visualAngle");
	static_assert(offsetof(Bullet, visualWidth) + 4 == offsetof(Bullet, visualLength),
		"Offset of visualHeight in Bullet must be exactly 4 more than that of visualWidth");
	extern const char* BL_VERTEX_SOURCE;
	extern const char* BL_FRAGMENT_SOURCE;
	class BulletList: public Collidable {
	public:
		BulletList(Playfield* p, agl::Texture shotsheet, int cc) :
			p(p), shotsheet(shotsheet), cc(cc) {}
		void setUp();
		void tick();
		void update();
		void _tearDown();
    int count() { return bullets.size(); }
    int strength() { return bullets.size(); }
    bool check(Circle& h);
    bool check(Line& h);
	 // TODO implement these
		void updatePositions(agl::IRect16& bounds);
    int collisionClass() { return cc; }
	private:
		std::vector<Bullet> bullets;
		Playfield* p;
		agl::Texture shotsheet;
		int cc;
		uint64_t highestID = 0;
		agl::VBO vbo;
		agl::VBO instanceVBO;
		agl::VAO vao;
		agl::ShaderProgram program;
		bool hasSetUniforms;
		bool hasInitializedProgram;
		void setUniforms();
	};
}
