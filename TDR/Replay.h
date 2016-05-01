#pragma once

#include <stdint.h>
#include <time.h>
#include <map>
#include <vector>

#include "CommonData.h"

namespace tdr {
	class ReplaySection {
	public:
		ReplaySection();
		~ReplaySection();
	private:
		CommonData cd;
		std::vector<uint64_t> keypresses;
		uint64_t functionalSeed;
		uint64_t aestheticSeed;
	};
	class Replay {
	public:
		Replay();
		~Replay();
	private:
		std::string name;
		time_t timestamp;
		std::map<int, ReplaySection> sections;
	};
}


