#pragma once

#include <map>
#include <memory>
#include <cstdint>
#include "Renderable.h"

namespace agl {
	class LayeredPriorityRenderable : Renderable {
	public:
		LayeredPriorityRenderable();
		~LayeredPriorityRenderable();
		void setUp();
		void tick();
		void update();
		void attach(uint32_t priority, std::shared_ptr<Renderable> r);
	protected:
		void _tearDown();
	private:
		std::unique_ptr<std::multimap<uint32_t, std::shared_ptr<Renderable>>> renderables;
	};
}
