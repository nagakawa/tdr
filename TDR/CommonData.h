#pragma once

#include <stdint.h>
#include <map>
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
		std::map<std::string, uint64_t> integers;
		std::map<std::string, double> doubles;
		std::map<std::string, std::string> strings;
	};
}
