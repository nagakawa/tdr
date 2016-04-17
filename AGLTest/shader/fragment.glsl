#version 330 core

// in vec3 ourColor;
in vec2 TexCoord;

out vec4 color;

uniform sampler2D ourTexture;
uniform sampler2D ourTexture2;
uniform float m;

void main() {
	color = mix(texture(ourTexture, TexCoord), texture(ourTexture2, TexCoord), m);
}