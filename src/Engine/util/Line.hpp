#pragma once

struct Line {
	constexpr explicit Line(const vec3& a, const vec3& b) : a(a), b(b) { }

    vec3 a {};
	vec3 b {};
};