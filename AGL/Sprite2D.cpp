#include "Sprite2D.h"

using namespace agl;

Sprite2D::Sprite2D(std::shared_ptr<Texture> t) {
	texture = t;
	sprites = new std::vector<Sprite2DInfo>;
}

Sprite2D::~Sprite2D() {
	delete vbo;
	delete instanceVBO;
	delete vao;
	delete ebo;
	delete program;
	delete sprites;
}

void agl::Sprite2D::setUp() {
	vbo = new VBO();
	instanceVBO = new VBO();
	ebo = new EBO();
	if (program == nullptr) {
		Shader vertex(VERTEX_SOURCE, GL_VERTEX_SHADER);
		Shader fragment(FRAGMENT_SOURCE, GL_FRAGMENT_SHADER);
		program = new ShaderProgram();
		program->attach(vertex);
		program->attach(fragment);
		program->link(); // There you go!
	}
	vao = new VAO();
	vao->setActive();
	vbo->setActive();
	// Vertex data
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*) 0);
	ebo->setActive();
	// Element data
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
	instanceVBO->setActive();
	// Instance data
	glBufferData(GL_ARRAY_BUFFER, sprites->size() * sizeof(Sprite2DInfo), sprites, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Sprite2DInfo), (GLvoid*) offsetof(Sprite2DInfo, source));
	glVertexAttribDivisor(0, 1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Sprite2DInfo), (GLvoid*) offsetof(Sprite2DInfo, dest));
	glVertexAttribDivisor(1, 1);
}

void agl::Sprite2D::tick() {
	glEnable(GL_BLEND);
	program->use();
	setTexture(texture);
	vao->setActive();
	glDrawElementsInstanced(GL_TRIANGLES, 2, GL_UNSIGNED_INT, 0, sprites->size());
}

void agl::Sprite2D::setTexture(std::shared_ptr<Texture> tex) {
	texture = tex;
	texture->bind();
	SETUNSP(*program, 1i, "tex", 0);
	SETUNSP2(*program, 2f, "texDimensions", tex->getWidth(), tex->getHeight());
	SETUNSP2(*program, 2f, "screenDimensions", app->getWidth(), app->getHeight);
}

void agl::Sprite2D::update() {
	instanceVBO->setActive();
	glBufferData(GL_ARRAY_BUFFER, sprites->size() * sizeof(Sprite2DInfo), sprites, GL_DYNAMIC_DRAW);
}

int agl::Sprite2D::addSprite(Sprite2DInfo loc) {
	int size = sprites->size();
	sprites->push_back(loc);
	update();
	return size;
}

void agl::Sprite2D::_tearDown() {
	delete sprites;
	delete vbo;
	delete instanceVBO;
	delete vao;
	delete ebo;
}
