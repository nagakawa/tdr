#version 330 core

layout (location = 0) in vec2 quad;
layout (location = 1) in ivec4 bounds;
layout (location = 2) in vec2 pos;
layout (location = 3) in vec2 glyphSize;
out vec2 tc;
uniform vec2 texDimensions;
uniform vec2 screenDimensions;
// NDC
uniform vec2 globalOffset;

void main() {
  tc = mix(bounds.xy, bounds.zw, quad) / texDimensions;
  vec2 realPos = pos + quad * glyphSize;
  realPos /= screenDimensions;
  realPos *= 2.0;
  realPos += globalOffset;
  gl_Position = vec4(realPos, 1.0f, 1.0f);
}
