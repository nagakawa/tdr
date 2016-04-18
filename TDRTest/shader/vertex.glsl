#version 330 core

// Vertex shader maps the vertex's data.
// Will be run on each vertex, so
// we need to specify the attributes of
// only one vertex.

layout (location = 0) in vec3 position;
// layout (location = 1) in vec3 color;
layout (location = 1) in vec2 texCoord;

// out vec3 ourColor;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	gl_Position = projection * view * model * vec4(position, 1.0);
	// ourColor = color;
	TexCoord = vec2(texCoord.x, 1.0f - texCoord.y);
}