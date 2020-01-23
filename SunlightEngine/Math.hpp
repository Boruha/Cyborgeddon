#pragma once

#include <cmath>

namespace Sun {
	constexpr double PI = 3.141592653589793238463;
	constexpr double DEG2RAD = PI / 180.0;
	constexpr double RAD2DEG = 180.0 / PI;

	constexpr float  EPSILON = 0.001f;

	inline bool equal_e(const float value, const float toCompare) { return std::abs(value - toCompare) < EPSILON; }
	inline bool greater_e(const float value, const float toCompare) { return (value - toCompare) > EPSILON; }
	inline bool less_e(const float value, const float toCompare) { return (value - toCompare) < -EPSILON; }

	constexpr inline float nearestAngle(const float first, const float second) {
		return first + static_cast<float>(std::fmod((std::fmod((std::fmod(second - first, 360) + 540), 360) - 180), 360));
	}
}
// TODO: precalcular cosas necesarias y recurrentes