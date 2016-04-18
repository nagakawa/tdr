#include "LayeredPriorityRenderable.h"

LayeredPriorityRenderable::LayeredPriorityRenderable() {
	renderables = std::unique_ptr<std::multimap<uint32_t, std::shared_ptr<Renderable>>>(new std::multimap<uint32_t, std::shared_ptr<Renderable>>);
	setUp();
}

LayeredPriorityRenderable::~LayeredPriorityRenderable() {
	renderables.reset();
	tearDown();
}

void LayeredPriorityRenderable::setUp() {
}

void LayeredPriorityRenderable::tearDown() {
	for (auto it = renderables->begin(); it != renderables->end(); ++it) {
		std::shared_ptr<Renderable> r = it->second;
		r->tearDown();
	}
}

void LayeredPriorityRenderable::tick() {
	for (auto it = renderables->begin(); it != renderables->end(); ++it) {
		std::shared_ptr<Renderable> r = it->second;
		r->tick();
	}
}

void LayeredPriorityRenderable::update() {
	for (auto it = renderables->begin(); it != renderables->end(); ++it) {
		std::shared_ptr<Renderable> r = it->second;
		r->update();
	}
}

void LayeredPriorityRenderable::attach(uint32_t priority, std::shared_ptr<Renderable> r) {
	renderables->insert(std::pair<uint32_t, std::shared_ptr<Renderable>>(priority, r));
}

