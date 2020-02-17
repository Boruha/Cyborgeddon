#pragma once

struct Line {
    Line() = default;
    explicit Line(const vec3& a, const vec3& b) : a(a), b(b) { }

    vec3 a, b;
};