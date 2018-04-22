#pragma once

#include <assert.h>
#include <stdint.h>
#include <functional>
#include <memory>
#include <vector>

#include <BlendMode.h>
// #include <Dimensional.h>
#include <FBO.h>
#include <GLFWApplication.h>
#include <rect.h>
#include <Renderable.h>
#include <Shader.h>
#include <ShaderProgram.h>
#include <VAO.h>
#include <VBO.h>
#include <kozet_fixed_point/kfp.h>
#include <zekku/BoxQuadTree.h>
#include <zekku/kfp_interop/timath.h>
#include <plf_colony.h>
#include "Playfield.h"

namespace tdr {
	using Circle = zekku::Circle<kfp::s16_16>;
	using Line = zekku::Line<kfp::s16_16>;
	struct Graphic {
		agl::UIRect16 texcoords;
		kfp::s16_16 visualRadius;
		kfp::s16_16 collisionRadius;
	};
	class Bullet {
	public:
		union {
			Circle c;
			Line l;
		} hitbox;
		kfp::s16_16 xs, ys;
		kfp::s16_16 xa, ya;
		kfp::s16_16 speed;
		kfp::frac32 angle, angularVelocity, visualAngle;
		// Width and length would be the same for ordinary bullets,
		// but different for lasers.
		kfp::s16_16 visualWidth, visualLength;
		agl::UIRect16 texcoords;
		uint16_t refcount = 0;
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
		uint8_t alpha = 255; // TODO this should be reflected in drawing code
		uint8_t delay;
		uint8_t isLaser;
		uint8_t bmIndex;
		bool markedForDeletion;
		// If true, this bullet will base xs and ys from speed and angle.
		// Otherwise, speed and angle depend on xs and ys.
		bool useRadial;
		bool detachVisualAndMovementAngles;
		bool deleteWhenOutOfBounds;
		bool collides;
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
		// Use tdr::Bullet::insert for this.
		Bullet(
				kfp::s16_16 x, kfp::s16_16 y,
				kfp::s16_16 speed, kfp::frac32 angle,
				Graphic& graph, uint8_t delay) : // CreateShotA1,
			hitbox{zekku::Circle<kfp::s16_16>()},
			xs(0), ys(0), xa(0), ya(0),
			speed(speed), angle(angle), angularVelocity(0),
			visualWidth(graph.visualRadius), visualLength(graph.visualRadius),
			texcoords(graph.texcoords), delay(delay), isLaser(false),
			markedForDeletion(false), useRadial(true),
			detachVisualAndMovementAngles(false),
			deleteWhenOutOfBounds(true), collides(true) {
			hitbox.c.c.x = x;
			hitbox.c.c.y = y;
			hitbox.c.r = graph.collisionRadius;
		}
		// dummy constructor to allow resize() on vector<Bullet>
		Bullet();
	};
	class BulletRenderInfo {
		union {
			Circle c;
			Line l;
		} hitbox;
		kfp::frac32 viualAngle;
		agl::UIRect16 texcoords;
		kfp::s16_16 visualRadius;
		bool isLaser;
	};
	class BulletHandle {
	public:
		BulletHandle(plf::colony<Bullet>::iterator it) : info(it) {
			++info->refcount;
		}
		BulletHandle(const BulletHandle& h) : info(h.info) {
			++info->refcount;
		}
		BulletHandle(BulletHandle&& h) {
			std::swap(info, h.info);
		}
		~BulletHandle() {
			--info->refcount; // Leave BulletList to clean defunct bullets
		}
		Bullet& operator*() { return *info; }
		const Bullet& operator*() const { return *info; }
		Bullet& operator->() { return *info; }
		const Bullet& operator->() const { return *info; }
	private:
		plf::colony<Bullet>::iterator info;
	};
	static_assert(offsetof(Bullet, visualAngle) + 4 == offsetof(Bullet, visualWidth),
		"Offset of visualWidth in Bullet must be exactly 4 more than that of visualAngle");
	static_assert(offsetof(Bullet, visualWidth) + 4 == offsetof(Bullet, visualLength),
		"Offset of visualHeight in Bullet must be exactly 4 more than that of visualWidth");
	extern const char* BL_VERTEX_SOURCE;
	extern const char* BL_FRAGMENT_SOURCE;
	// TODO: support multiple render passes for BulletList
	// This will be useful not only for avoiding multiple BulletLists if
	// we wish to have bullets of different blend modes, but also handle
	// the case where a bullet must be rendered with different blend modes.
	// These cases are seen in:
	// * the TLB of Book of Star Mythology -- the first phase has black
	//   bullets (ALPHA) with white auras around them (ADD)
	// (any other examples?)
	class BulletList {
	public:
		BulletList(
				std::shared_ptr<Playfield> p,
				std::shared_ptr<agl::Texture> shotsheet) :
			p(p), shotsheet(shotsheet) {}
		void setUp();
		void tick();
		void update();
		void _tearDown();
    bool check(const Circle& h);
    bool check(const Line& h);
		void updatePositions(const agl::IRect16& bounds);
		void insert(Bullet& b);
		BulletHandle createShotA1(
			kfp::s16_16 x, kfp::s16_16 y,
			kfp::s16_16 speed, kfp::frac32 angle,
			Graphic& graph, uint8_t delay);
		void graze(
			const Circle& h,
			std::function<void(Bullet&)> callback);
	private:
		plf::colony<Bullet> bullets;
		std::vector<BulletRenderInfo> rinfo;
		std::shared_ptr<Playfield> p;
		std::shared_ptr<agl::Texture> shotsheet;
		agl::VBO vbo;
		agl::VBO instanceVBO;
		agl::VAO vao;
		agl::ShaderProgram program;
		bool hasSetUniforms;
		bool hasInitialisedProgram;
		void setUniforms();
	};
}
