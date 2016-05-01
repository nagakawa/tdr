#pragma once

#define DEF_SET(sn, tn, fn) \
	tn (get##sn)(const std::string& key, tn def) { \
		if (fn.count(key)) return fn[key]; \
		return def; \
	} \
	void (set##sn)(const std::string& key, tn val) { \
		fn[key] = val; \
	}
