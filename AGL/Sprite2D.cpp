#include "Sprite2D.h"

using namespace agl;


const char* agl::VERTEX_SOURCE = "\
#version 330 core \n\
layout (location = 0) in vec2 pos; \n\
layout (location = 1) in vec4 source; \n\
layout (location = 2) in vec4 dest; \n\
out vec2 texCoord; \n\
uniform vec2 texDimensions; \n\
uniform vec2 screenDimensions; \n\
\n\
void main() { \n\
	vec2 tcinv = vec2(mix(source.x, source.z, pos.x), mix(source.y, source.w, pos.y)) / texDimensions; \n\
	texCoord = vec2(tcinv.x, tcinv.y); \n\
	vec2 position = vec2(mix(dest.x, dest.z, pos.x), mix(dest.y, dest.w, pos.y)) / screenDimensions; \n\
	gl_Position = vec4(position * vec2(2.0f, -2.0f) + vec2(-1.0f, 1.0f), 1.0f, 1.0f); \n\
} \
";
const char* agl::FRAGMENT_SOURCE = "\
#version 330 core \n\
in vec2 texCoord; \n\
out vec4 color; \n\
uniform sampler2D tex; \n\
\n\
void main() { \n\
	color = texture(tex, texCoord); \n\
} \
";

Sprite2D::Sprite2D(std::shared_ptr<Texture> t) {
	texture = t;
	sprites = new std::vector<Sprite2DInfo>;
	vbo = new VBO();
	instanceVBO = new VBO();
	vao = new VAO();
	app = nullptr;
	hasSetUniforms = false;
}

Sprite2D::~Sprite2D() {
	Renderable::~Renderable();
}

void agl::Sprite2D::setUp() {
	if (app == nullptr)
		throw "Current app must be set first";
	Shader vertex(VERTEX_SOURCE, GL_VERTEX_SHADER);
	Shader fragment(FRAGMENT_SOURCE, GL_FRAGMENT_SHADER);
	program = new ShaderProgram();
	program->attach(vertex);
	program->attach(fragment);
	program->link(); // There you go!
	vao->setActive();
	// Vertex data
	vbo->feedData(sizeof(vertices), (void*) vertices, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*) 0);
	// Instance data
	instanceVBO->setActive();
	instanceVBO->feedData(sprites->size() * sizeof(Sprite2DInfo), sprites->data(), GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Sprite2DInfo), (GLvoid*) offsetof(Sprite2DInfo, source));
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Sprite2DInfo), (GLvoid*) offsetof(Sprite2DInfo, dest));
	resetVBO();
	glVertexAttribDivisor(1, 1);
	glVertexAttribDivisor(2, 1);
	resetVAO();
}

void agl::Sprite2D::tick() {
	glEnable(GL_BLEND);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	setTexture(texture);
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 6, sprites->size());
	resetVAO();
}

void agl::Sprite2D::setTexture(std::shared_ptr<Texture> tex) {
	vao->setActive();
	program->use();
	tex->bindTo(0);
	if (app == nullptr) throw "App must be set first";
	if (texture == tex && hasSetUniforms) return;
	texture = tex;
	SETUNSP(*program, 1i, "tex", 0);
	SETUNSP2(*program, 2f, "texDimensions", (GLfloat) tex->getWidth(), (GLfloat) tex->getHeight());
	SETUNSP2(*program, 2f, "screenDimensions", (GLfloat) app->getWidth(), (GLfloat) app->getHeight());
	hasSetUniforms = true;
}

void agl::Sprite2D::update() {
	instanceVBO->feedData(sprites->size() * sizeof(Sprite2DInfo), sprites->data(), GL_DYNAMIC_DRAW);
}

int agl::Sprite2D::addSprite(Sprite2DInfo loc) {
	int size = sprites->size();
	sprites->push_back(loc);
	if (program != nullptr) update();
	return size;
}

void agl::Sprite2D::_tearDown() {
	delete sprites;
	delete vbo;
	delete instanceVBO;
	delete vao;
}
