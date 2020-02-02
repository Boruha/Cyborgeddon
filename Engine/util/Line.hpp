#pragma once

#include <Engine/util/Vector3.hpp>

struct Line {
    Line() = default;
    explicit Line(const Vector3f& a, const Vector3f& b) : a(a), b(b) { }

    Vector3f a, b;
};