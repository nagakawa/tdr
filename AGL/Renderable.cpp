#include "Renderable.h"

using namespace agl;

Renderable::Renderable() {
	setUp();
}

Renderable::~Renderable() {
	tearDown();
}

void Renderable::setUp() {
}

void Renderable::tearDown() {
	_tearDown();
	tornDown = true;
}

void Renderable::tick() {
}

void Renderable::update() {
}

void Renderable::_tearDown() {
}
