#pragma once

#include <Engine/util/Vector3.hpp>

struct Plane {
    explicit Plane(const Vector3f& n, const float d = 0) : normal(n), distance(d) { }

    const Vector3f    normal;
    const float     distance;
};