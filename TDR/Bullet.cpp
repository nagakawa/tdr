#include "Bullet.h"

#include <math.h>
#include <algorithm>
#include <kozet_fixed_point/kfp.h>
#include <kozet_fixed_point/kfp_extra.h>
#include <BlendMode.h>

using namespace tdr;

void tdr::Bullet::update() {
	if (useRadial) {
		kfp::s2_30 ac, as;
		kfp::sincos(angle, ac, as);
		xs = speed * ac;
		ys = speed * as;
	} else {
		kfp::rectp(xs, ys, speed, angle);
	}
	hitbox.c.c.x += xs;
	hitbox.c.c.y += ys;
	xs += xa;
	ys += ya;
	if (delay != 0) --delay;
	if (!detachVisualAndMovementAngles) visualAngle = angle;
	refreshGraze();
}

bool tdr::Bullet::graze() {
	if (timeToNextGraze != 0 || grazeFrequency == -1) return false;
	timeToNextGraze = grazeFrequency == 0 ? -1 : grazeFrequency - 1;
	return true;
}

void tdr::Bullet::refreshGraze() {
	if (timeToNextGraze != 0 && grazeFrequency != -1) --timeToNextGraze;
}

const char* tdr::BL_VERTEX_SOURCE = "\
#version 330 core \n\
layout (location = 0) in vec2 bounds; \n\
layout (location = 1) in vec4 hitbox; \n\
layout (location = 2) in vec3 awl; \n\
layout (location = 3) in vec4 shottc; \n\
out vec2 texCoord; \n\
uniform vec2 texDimensions; \n\
uniform vec2 screenDimensions; \n\
\n\
void main() { \n\
	texCoord = vec2(mix(shottc.x, shottc.z, bounds.x), mix(shottc.y, shottc.w, bounds.y)) / texDimensions; \n\
	float angle = awl.x * 2 * 3.14159265358979323 / 16384; \n\
	mat2 rm = mat2(cos(angle), -sin(angle), sin(angle), cos(angle)); \n\
	vec2 pos = position / screenDimensions + rm * ((bounds * vec2(2.0f, 2.0f) - vec2(1.0f, 1.0f)) * awl.zy); \n\
	gl_Position = vec4(position * vec2(2.0f, -2.0f) + vec2(-1.0f, 1.0f), 1.0f, 1.0f); \n\
} \
";
const char* tdr::BL_FRAGMENT_SOURCE = "\
#version 330 core \n\
in vec2 texCoord; \n\
out vec4 colour; \n\
uniform sampler2D tex; \n\
\n\
void main() { \n\
	colour = texture(tex, texCoord); \n\
} \
";

void tdr::BulletList::setUp() {
	if (p == nullptr)
		throw "What, are you crazy?!";
	rinfo.resize(agl::BMIDX_COUNT);
	agl::Shader vertex(BL_VERTEX_SOURCE, GL_VERTEX_SHADER);
	agl::Shader fragment(BL_FRAGMENT_SOURCE, GL_FRAGMENT_SHADER);
	program.attach(vertex);
	program.attach(fragment);
	program.link();
	hasInitialisedProgram = true;
	vao.setActive();
	// Vertex data
	vbo.feedData(sizeof(agl::rectangleVertices), (void*) agl::rectangleVertices, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*) 0);
	// InstanceData
	update();
	// The shader needs to know about the following fields:
	// hitbox.c.c.x hitbox.c.c.y
	// or all four fields of hitbox.l if isLaser is set
	// visualAngle visualWidth visualLength
	// texcoords
	// So, without further ado:
	glEnableVertexAttribArray(1);
	// Would've loved to use GL_FIXED for this and be done with that,
	// but sadly, it's GL4.2+.
	glVertexAttribIPointer(
		1, 4, GL_INT, sizeof(BulletRenderInfo),
		(GLvoid*) (offsetof(BulletRenderInfo, hitbox))
	);
	glVertexAttribDivisor(1, 1);
	glEnableVertexAttribArray(2);
	glVertexAttribIPointer(
		2, 2, GL_INT, sizeof(BulletRenderInfo),
		(GLvoid*) (offsetof(BulletRenderInfo, visualAngle))
	);
	glVertexAttribDivisor(2, 1);
	glEnableVertexAttribArray(3);
	glVertexAttribIPointer(
		3, 4, GL_SHORT, sizeof(Bullet),
		(GLvoid*) (offsetof(BulletRenderInfo, texcoords))
	);
	glVertexAttribDivisor(3, 1);
	agl::resetVBO();
	agl::resetVAO();
}

void tdr::BulletList::setUniforms() {
	if (hasSetUniforms) return;
	vao.setActive();
	program.use();
	shotsheet->bindTo(0);
	SETUNSP(program, 1i, "tex", 0);
	SETUNSP2(program, 2f, "texDimensions", (GLfloat) shotsheet->getWidth(), (GLfloat) shotsheet->getHeight());
	SETUNSP2(program, 2f, "screenDimensions", (GLfloat) p->getWidth(), (GLfloat) p->getHeight());
	hasSetUniforms = true;
}

void tdr::BulletList::render() {
	p->getFBO().setActive();
	glEnable(GL_BLEND);
	agl::BM_ALPHA.use();
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	setUniforms();
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, bullets.size());
	agl::resetVAO();
}

void tdr::BulletList::update() {
	// Set data area
	spurt();
	instanceVBO.feedData(
		offsets.back() * sizeof(BulletRenderInfo),
		nullptr,
		GL_DYNAMIC_DRAW);
	// Set individual chunks
	for (size_t i = 0; i < rinfo.size(); ++i) {
		instanceVBO.feedSubdata(
			offsets[i] * sizeof(BulletRenderInfo),
			(offsets[i + 1] - offsets[i]) * sizeof(BulletRenderInfo),
			rinfo[i].data()
		);
	}
}

void tdr::BulletList::_tearDown() {
}

bool tdr::BulletList::check(const Circle& h) {
	for (Bullet& b : bullets) {
		if (!b.collides) continue;
		if (b.isLaser ?
				b.hitbox.l.intersects(h) :
				b.hitbox.c.intersects(h))
			return true;
	}
	return false;
}

bool tdr::BulletList::check(const Line& h) {
	for (Bullet& b : bullets) {
		if (!b.collides) continue;
		if (b.isLaser ?
				b.hitbox.l.intersects(h) :
				b.hitbox.c.intersects(h))
			return true;
	}
	return false;
}

void tdr::BulletList::updatePositions(const agl::IRect16& bounds) {
	for (Bullet& b : bullets) {
		b.update();
		if (b.deleteWhenOutOfBounds && 
				b.hitbox.c.isWithin({
					{
						kfp::s16_16(bounds.left + bounds.right) / 2,
						kfp::s16_16(bounds.top + bounds.bottom) / 2,
					},
					{
						kfp::s16_16(bounds.right - bounds.left) / 2,
						kfp::s16_16(bounds.bottom - bounds.top) / 2,
					}
				}))
			b.markedForDeletion = true;
	}
	// Remove marked bullets with no references
	std::remove_if(bullets.begin(), bullets.end(), [](const Bullet& b) {
		return b.markedForDeletion && b.refcount == 0;
	});
}

void tdr::BulletList::graze(
		const Circle& h,
		std::function<void(Bullet&)> callback) {
	for (Bullet& b : bullets) {
		if (!b.collides) continue;
		if (b.isLaser ?
				b.hitbox.l.intersects(h) :
				b.hitbox.c.intersects(h)) {
			b.graze();
			callback(b);
		}
	}
}

BulletHandle tdr::BulletList::createShotA1(
		kfp::s16_16 x, kfp::s16_16 y,
		kfp::s16_16 speed, kfp::frac32 angle,
		Graphic& graph, uint8_t delay) {
	auto it = bullets.emplace(x, y, speed, angle, graph, delay);
	return BulletHandle(it);
}

void tdr::BulletList::spurt() {
	// Spurts the render-related data from the `bullets` colony
	// to the `rinfo` vector. This vector will then be used by
	// the render code to draw the bullets.
	for (auto& ri : rinfo) ri.clear();
	for (const Bullet& b : bullets) {
		// Don't render bullets that are marked for deletion
		if (b.markedForDeletion) continue;
		// Where should we push?
		size_t bmidx = b.bmIndex;
		auto& toPush = rinfo[bmidx];
		toPush.emplace_back();
		BulletRenderInfo& entry = toPush.back();
		entry.isLaser = b.isLaser;
		if (b.isLaser) entry.hitbox.l = b.hitbox.l;
		else entry.hitbox.c = b.hitbox.c;
		entry.visualAngle = b.visualAngle;
		entry.texcoords = b.texcoords;
		entry.visualRadius = b.visualWidth;
	}
	// Update offsets vector
	offsets.resize(rinfo.size() + 1);
	offsets[0] = 0;
	for (size_t i = 0; i < rinfo.size(); ++i)
		offsets[1 + i] = offsets[i] + rinfo[i].size();
}
