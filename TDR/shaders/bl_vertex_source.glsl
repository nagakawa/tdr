#version 330 core
layout (location = 0) in vec2 bounds;
layout (location = 1) in vec4 hitbox;
layout (location = 2) in vec2 visualAngleAndRadius;
layout (location = 3) in vec4 shottc;
layout (location = 4) in int isLaser;
out vec2 texCoord;
uniform vec2 texDimensions;
uniform vec2 screenDimensions;
void main() {
	texCoord = vec2(
		mix(shottc.x, shottc.z, bounds.x),
		mix(shottc.y, shottc.w, bounds.y)) / texDimensions;
	float angle = awl.x * 2 * 3.14159265358979323 / 16384;
	mat2 rm = mat2(cos(angle), -sin(angle), sin(angle), cos(angle));
	vec2 pos = position / screenDimensions + rm * ((bounds * vec2(2.0f, 2.0f) - vec2(1.0f, 1.0f)) * awl.zy);
	gl_Position = vec4(position * vec2(2.0f, -2.0f) + vec2(-1.0f, 1.0f), 1.0f, 1.0f);
}
