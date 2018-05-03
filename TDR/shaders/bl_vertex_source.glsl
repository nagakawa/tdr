#version 330 core

layout (location = 0) in vec2 bounds;
layout (location = 1) in ivec4 hitbox;
layout (location = 2) in ivec2 visualAngleAndRadius;
layout (location = 3) in ivec4 shottc;
layout (location = 4) in int isLaser;
out vec2 texCoord;
uniform vec2 texDimensions;
uniform vec2 screenDimensions;

const float fixedTurnsToRadians =
  2 * 3.14159265358979323 / 4294967296.0;

void main() {
	texCoord = vec2(
		mix(shottc.x, shottc.z, bounds.x),
		mix(shottc.y, shottc.w, bounds.y)) / texDimensions;
	float angle = visualAngleAndRadius.x * fixedTurnsToRadians;
	mat2 rm = mat2(cos(angle), -sin(angle), sin(angle), cos(angle));
  vec2 nbounds = bounds * vec2(2.0f, 2.0f) - vec2(1.0f, 1.0f);
  vec2 position = hitbox.xy;
	vec2 pos =
    position + rm * (nbounds * visualAngleAndRadius.y);
  // Normalise to [0, 1]
  pos /= 65536;
  pos /= screenDimensions;
  // Normalise to [-1, 1]
  pos = pos * vec2(2.0f, -2.0f) + vec2(-1.0f, 1.0f);
  // Flip vertically
  pos.y *= -1;
	gl_Position = vec4(pos, 1.0f, 1.0f);
}
