#pragma once

struct Plane {
	constexpr explicit Plane(const vec3& n, const float d = 0) : normal(n), distance(d) { }

    const vec3    normal;
    const float   distance;
};