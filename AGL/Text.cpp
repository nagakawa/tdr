#include "Text.h"

using namespace agl;

const char* agl::TXT_VERTEX_SOURCE = "\
#version 330 core \n\
layout (location = 0) in vec2 pos; \n\
out vec2 texCoord; \n\
out vec2 texSize; \n\
uniform vec2 topLeft; \n\
uniform vec2 texDimensions; \n\
uniform vec2 screenDimensions; \n\
\n\
void main() { \n\
	texSize = texDimensions; \n\
	texCoord = pos; \n\
	vec2 position = (topLeft + pos * texSize) / screenDimensions; \n\
	gl_Position = vec4(position * vec2(2.0f, -2.0f) + vec2(-1.0f, 1.0f), 1.0f, 1.0f); \n\
} \
";
const char* agl::TXT_FRAGMENT_SOURCE = "\
#version 330 core \n\
in vec2 texCoord; \n\
in vec2 texSize; \n\
out vec4 color; \n\
uniform sampler2D tex; \n\
uniform vec4 topColor; \n\
uniform vec4 bottomColor; \n\
uniform float scale; \n\
\n\
void main() { \n\
	vec2 e1 = vec2(min(1, 0.5 * scale), 0) / texSize; \n\
	vec2 e2 = vec2(0, min(1, 0.5 * scale)) / texSize; \n\
	float a = texture(tex, texCoord).a; \n\
	a = (a - 205.0 / 255) * 255 / 50; \n\
	float an = texture(tex, texCoord + e2).a; \n\
	float as = texture(tex, texCoord - e2).a; \n\
	float aw = texture(tex, texCoord - e1).a; \n\
	float ae = texture(tex, texCoord + e1).a; \n\
	float ann = texture(tex, texCoord + 2 * e2).a; \n\
	float ass = texture(tex, texCoord - 2 * e2).a; \n\
	float aww = texture(tex, texCoord - 2 * e1).a; \n\
	float aee = texture(tex, texCoord + 2 * e1).a; \n\
	float av = (an + as + aw + ae + ann + ass + aww + aee) * 0.125; \n\
	av = (av - 205.0 / 255) * 255 / 50; \n\
	a = a + (av - 0.5) * (1 - a); \n\
	a = clamp(a, 0, 1); \n\
	color = vec4(texture(tex, texCoord).rgb, a) * mix(bottomColor, topColor, texCoord.y); \n\
} \
";

Text::Text() {
	texture = new Texture();
	width = 0;
	height = 0;
	margin = 999;
	size = 1;
	app = nullptr;
	hasInitializedProgram = false;
	topColor = bottomColor = glm::vec4(0, 0, 0, 1);
	position = glm::vec2(0, 0);
	font = "VL Gothic";
}

Text::~Text() {
	delete texture;
}

GLfloat TEXT_BORDER_COLOR[] = { 1.0f, 1.0f, 1.0f, 0.0f };

void agl::Text::setText(std::string txt, bool relayout) {
	if (relayout) {
		renderText(txt.c_str(), font.c_str(), width, height, margin, size, *texture);
		texture->bind();
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, TEXT_BORDER_COLOR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	}
	text = txt;
}

void agl::Text::setUp() {
	if (app == nullptr)
		throw "Current app must be set first!";
	Shader vertex(TXT_VERTEX_SOURCE, GL_VERTEX_SHADER);
	Shader fragment(TXT_FRAGMENT_SOURCE, GL_FRAGMENT_SHADER);
	program.attach(vertex);
	program.attach(fragment);
	program.link(); // There you go!
	hasInitializedProgram = true;
	vao.setActive();
	// Vertex data
	vbo.feedData(sizeof(rectangleVertices), (void*) rectangleVertices, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*) 0);
	resetVBO();
	resetVAO();
}

void agl::Text::tick() {
	glEnable(GL_BLEND);
	BM_ALPHA.use();
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	vao.setActive();
	program.use();
	texture->bindTo(0);
	if (app == nullptr) throw "App must be set first";
	SETUNSP(program, 1i, "tex", 0);
	SETUNSP(program, 1f, "scale", (GLfloat) size);
	SETUNSP2(program, 2f, "topLeft", position.x, position.y);
	SETUNSP2(program, 2f, "texDimensions", (GLfloat) width, (GLfloat) height);
	SETUNSP2(program, 2f, "screenDimensions", (GLfloat) app->getWidth(), (GLfloat) app->getHeight());
	SETUNSPV(program, 4fv, "topColor", topColor.data);
	SETUNSPV(program, 4fv, "bottomColor", bottomColor.data);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	resetVAO();
}

void agl::Text::update() {
}

void agl::Text::_tearDown() {
}
