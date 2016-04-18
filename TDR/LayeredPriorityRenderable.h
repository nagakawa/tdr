#pragma once

#include <map>
#include <memory>
#include <cstdint>
#include "Renderable.h"

class LayeredPriorityRenderable: Renderable {
public:
	LayeredPriorityRenderable();
	~LayeredPriorityRenderable();
	void setUp();
	void tearDown();
	void tick();
	void update();
	void attach(uint32_t priority, std::shared_ptr<Renderable> r);
private:
	std::multimap<uint32_t, std::shared_ptr<Renderable>>* renderables;
};

