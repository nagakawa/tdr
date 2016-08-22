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

const char* tdr::BL_VERTEX_SOURCE = "\
#version 330 core \n\
layout (location = 0) in vec2 bounds; \n\
layout (location = 1) in vec2 position; \n\
layout (location = 2) in vec3 awl; \n\
layout (location = 3) in vec4 shottc; \n\
out vec2 texCoord; \n\
uniform vec2 texDimensions; \n\
uniform vec2 screenDimensions; \n\
\n\
void main() { \n\
	texCoord = vec2(mix(shottc.x, shottc.z, bounds.x), mix(shottc.y, shottc.w, bounds.y)) / texDimensions; \n\
	float angle = awl.x * 2 * 3.14159265358979323; \n\
	mat2 rm = mat2(cos(angle), -sin(angle), sin(angle), cos(angle)); \n\
	vec2 pos = position + rm * (bounds * awl.yz); \n\
	gl_Position = vec4(position * vec2(2.0f, -2.0f) + vec2(-1.0f, 1.0f), 1.0f, 1.0f); \n\
} \
";
const char* tdr::BL_FRAGMENT_SOURCE = "\
#version 330 core \n\
in vec2 texCoord; \n\
out vec4 color; \n\
uniform sampler2D tex; \n\
\n\
void main() { \n\
	color = texture(tex, texCoord); \n\
} \
";

void tdr::BulletList::setUp() {
	if (p == nullptr)
		throw "What, are you crazy?!";
	agl::Shader vertex(BL_VERTEX_SOURCE, GL_VERTEX_SHADER);
	agl::Shader fragment(BL_FRAGMENT_SOURCE, GL_FRAGMENT_SHADER);
	program.attach(vertex);
	program.attach(fragment);
	program.link();
	hasInitializedProgram = true;
	vao.setActive();
	// Vertex data
	vbo.feedData(sizeof(agl::rectangleVertices), (void*) agl::rectangleVertices, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*) 0);
	// InstanceData
	instanceVBO.setActive();
	instanceVBO.feedData(bullets.size() * sizeof(Bullet), bullets.data(), GL_DYNAMIC_DRAW);
	// The shader needs to know about the following fields:
	// hitbox.c.x hitbox.c.y
	// visualAngle visualWidth visualLength
	// texcoords
	// So, without further ado:
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FIXED, false, sizeof(Bullet), (GLvoid*) offsetof(Bullet, hitbox.c.x));
	glVertexAttribDivisor(1, 1);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_UNSIGNED_INT, true, sizeof(Bullet), (GLvoid*) offsetof(Bullet, visualAngle));
	glVertexAttribDivisor(2, 1);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_SHORT, false, sizeof(Bullet), (GLvoid*) offsetof(Bullet, texcoords));
	glVertexAttribDivisor(3, 1);
	agl::resetVBO();
	agl::resetVAO();
}

void tdr::BulletList::setUniforms() {
	if (hasSetUniforms) return;
	vao.setActive();
	program.use();
	shotsheet.bindTo(0);
	SETUNSP(program, 1i, "tex", 0);
	SETUNSP2(program, 2f, "texDimensions", (GLfloat) shotsheet.getWidth(), (GLfloat) shotsheet.getHeight());
	SETUNSP2(program, 2f, "screenDimensions", (GLfloat) p->width(), (GLfloat) p->height());
	hasSetUniforms = true;
}

void tdr::BulletList::tick() {
	glEnable(GL_BLEND);
	agl::BM_ALPHA.use();
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	setUniforms();
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, bullets.size());
	agl::resetVAO();
}
