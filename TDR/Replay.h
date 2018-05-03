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
		bool read(uint64_t& keys, uint16_t& fps);
		void write(uint64_t keys, uint16_t fps);
		size_t tell() const { return idx; }
		void seek(size_t i) { idx = i; }
	private:
		CommonData cd;
		std::vector<uint64_t> keypresses;
		std::vector<uint16_t> fpses;
		uint64_t functionalSeed;
		uint64_t aestheticSeed;
		size_t idx = 0;
	};
	class Replay {
	public:
		Replay();
		~Replay();
	private:
		std::string name;
		time_t timestamp;
		std::map<int, ReplaySection> sections;
    int currentStage;
	};
}
