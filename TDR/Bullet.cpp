#include "Bullet.h"

using namespace tdr;

fix1616 tdr::operator+(fix1616 a, fix1616 b) {
	if (b.u > 0 && ((int32_t) 0xFFFFFFFF) - b.u < a.u)
		return { (int32_t) 0xFFFFFFFF };
	if (b.u < 0 && -((int32_t) 0xFFFFFFFF) - b.u > a.u)
		return { -((int32_t) 0xFFFFFFFF) };
	return {a.u + b.u};
}
fix1616 tdr::operator-(fix1616 a, fix1616 b) {
	if (b.u > 0 && -((int32_t) 0xFFFFFFFF) + b.u < a.u)
		return { -(int32_t) 0xFFFFFFFF };
	if (b.u < 0 && ((int32_t) 0xFFFFFFFF) + b.u > a.u)
		return { ((int32_t) 0xFFFFFFFF) };
	return { a.u - b.u };
}
fix1616 tdr::operator*(fix1616 a, fix1616 b) {
	int64_t prod = (((int64_t) a.u) * b.u) >> 16;
	if (prod >= 0x100000000) prod = 0xFFFFFFFF;
	if (prod < -((int64_t) 0x100000000)) prod = -((int64_t) 0xFFFFFFFF);
	return { (int32_t) prod };
}
fix1616 tdr::operator/(fix1616 a, fix1616 b) {
	int64_t aex = (((int64_t) a.u) << 16);
	int64_t res = aex / b.u;
	if (res > ((int64_t) 0xFFFFFFFF)) return { (int32_t) 0xFFFFFFFF };
	if (res < -((int64_t) 0xFFFFFFFF)) return { -((int32_t) 0xFFFFFFFF) };
	return { (int32_t) res };
}

