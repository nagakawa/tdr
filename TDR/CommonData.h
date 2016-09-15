#pragma once

#include <stdint.h>
#include <unordered_map>
#include <string>

#include "defset.h"

namespace tdr {
	class CommonData {
	public:
		CommonData();
		~CommonData();
		// yay more abuse of preprocessor macros
		DEF_SET(Int, uint64_t, integers)
		DEF_SET(Double, double, doubles)
		DEF_SET(String, std::string, strings)
	private:
		std::unordered_map<std::string, uint64_t> integers;
		std::unordered_map<std::string, double> doubles;
		std::unordered_map<std::string, std::string> strings;
	};
}
