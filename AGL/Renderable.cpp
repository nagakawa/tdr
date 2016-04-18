#include "Renderable.h"

Renderable::Renderable() {
	setUp();
}

Renderable::~Renderable() {
	tearDown();
}

void Renderable::setUp() {
}

void Renderable::tearDown() {
	tearDown();
	tornDown = true;
}

void Renderable::tick() {
}

void Renderable::update() {
}

void Renderable::_tearDown() {
}
