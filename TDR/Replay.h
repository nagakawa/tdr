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
		void setRNGSeed(uint64_t seed) {
			this->seed = seed;
		}
		uint64_t getRNGSeed() { return seed; }
	private:
		CommonData cd;
		std::vector<uint64_t> keypresses;
		std::vector<uint16_t> fpses;
		uint64_t seed;
		size_t idx = 0;
	};
	class Replay {
	public:
		Replay();
		~Replay();
		int getCurrentStage() { return currentStage; }
		void setCurrentStage(int stage) { currentStage = stage; }
		ReplaySection& currentSection() { return sections[currentStage]; }
		time_t getTimestamp() { return timestamp; }
		void setTimestamp(time_t t) { timestamp = t; }
	private:
		std::string name;
		time_t timestamp;
		std::map<int, ReplaySection> sections;
    int currentStage;
	};
}
