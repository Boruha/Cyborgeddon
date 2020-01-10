#pragma once

#include <cmath>

namespace Sun {
	constexpr double PI = 3.1415926535897932384626433832795028841971693993751;
	constexpr double DEG2RAD = PI / 180.0;
	constexpr double RAD2DEG = 180.0 / PI;
	constexpr int    FIX_ERROR_FACTOR = 65536;
	constexpr float  EPSILON = 0.001f;

	inline bool equal_e(const float value, const float toCompare) { return std::abs(value - toCompare) < EPSILON; }
}
// TODO: precalcular cosas necesarias y recurrentes