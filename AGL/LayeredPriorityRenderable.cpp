#include "LayeredPriorityRenderable.h"

#define GLEW_STATIC
#include <GL/glew.h>

using namespace agl;

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

void LayeredPriorityRenderable::_tearDown() {
	for (auto it = renderables->begin(); it != renderables->end(); ++it) {
		std::shared_ptr<Renderable> r = it->second;
		r->tearDown();
	}
}

void LayeredPriorityRenderable::tick() {
	uint32_t prev = 0;
	for (auto it = renderables->begin(); it != renderables->end();) {
		uint32_t prio = it->first;
		std::shared_ptr<Renderable> r = it->second;
		if (prio != prev) glClear(GL_DEPTH_BUFFER_BIT);
		prev = prio;
		if (r->isTornDown()) {
			it = renderables->erase(it);
		} else {
			r->tick();
			++it;
		}
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

