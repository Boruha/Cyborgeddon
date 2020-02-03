#pragma once

#include <Engine/util/Plane.hpp>
#include <Engine/util/Line.hpp>

inline Vector3f intersectionPoint(const Plane& p, const Line& l) {
    return l.a + ((l.b - l.a) * ((p.distance - p.normal.dot(l.a)) / p.normal.dot(l.b - l.a)));
}