#pragma once

#define DEF_SET(sn, tn, fn) \
	tn (get##sn)(std::string key, tn def) { \
		if (fn.count(key)) return fn[key]; \
		return def; \
	} \
	void (set##sn)(std::string key, tn val) { \
		fn[key] = val; \
	}
