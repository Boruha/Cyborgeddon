#pragma once

#include <glm/glm.hpp>
#include <iostream>

using glm::vec2;
using glm::vec3;

constexpr double PI = 3.141592653589793238463;
constexpr double DEG2RAD = PI / 180.0;
constexpr double RAD2DEG = 180.0 / PI;

constexpr float  EPSILON = 0.0001f;

constexpr inline bool equal_e(const float value, const float toCompare) { return std::abs(value - toCompare) < EPSILON; }
constexpr inline bool greater_e(const float value, const float toCompare) { return (value - toCompare) > EPSILON; }
constexpr inline bool less_e(const float value, const float toCompare) { return (value - toCompare) < -EPSILON; }

constexpr inline float nearestAngle(const float first, const float second) {
    return first + static_cast<float>(std::fmod((std::fmod((std::fmod(second - first, 360) + 540), 360) - 180), 360));
}

constexpr inline float length2(const vec2& v) {
	return (v.x * v.x) + (v.y * v.y);
}

constexpr inline float length2(const vec3& v) {
	return (v.x * v.x) + (v.y * v.y) + (v.z * v.z);
}

constexpr inline float length(const vec2& v) {
	return std::sqrt( length2(v) );
}

constexpr inline float length(const vec3& v) {
	return std::sqrt( length2(v) );
}

constexpr inline vec2& normalize(vec2& v) {
	const float l = length(v);

	if (l != 0)
		v /= l;

	return v;
}

constexpr inline vec3& normalize(vec3& v) {
	const float l = length(v);

	if (l != 0)
		v /= l;

	return v;
}

constexpr inline float dot(const vec2& v1, const vec2& v2) {
	return (v1.x * v2.x) + (v1.y * v2.y);
}

constexpr inline float dot(const vec3& v1, const vec3& v2) {
	return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
}

constexpr inline float getRotationYfromXZ(const vec3& v) {
	return float(std::atan2(v.x, v.z) * RAD2DEG);
}

inline vec3 getXZfromRotationY(const double deg) {
	const double rad = deg * DEG2RAD;

	const auto x = float(sin(rad));
	const auto z = float(cos(rad));

	return vec3(x,0,z);
}

constexpr inline void abs(vec2& v) {
	v.x = std::abs(v.x);
	v.y = std::abs(v.y);
}

constexpr inline void abs(vec3& v) {
	v.x = std::abs(v.x);
	v.y = std::abs(v.y);
	v.z = std::abs(v.z);
}

inline void rotateXZ(vec3& v, const double deg, const vec3& origin = vec3()) {
	const double rad = deg * DEG2RAD;

	v.x -= origin.x;
	v.z -= origin.z;

	v.x = (v.x * cos(rad)) - (v.z * sin(rad));
	v.z = (v.x * sin(rad)) + (v.z * cos(rad));

	v.x += origin.x;
	v.z += origin.z;
}

inline void rotateXY(vec3& v, const double deg, const vec3& origin = vec3()) {
	const double rad = deg * DEG2RAD;

	v.x -= origin.x;
	v.y -= origin.y;

	v.x = (v.x * cos(rad)) - (v.y * sin(rad));
	v.y = (v.x * sin(rad)) + (v.y * cos(rad));

	v.x += origin.x;
	v.y += origin.y;
}

inline void rotateYZ(vec3& v, const double deg, const vec3& origin = vec3()) {
	const double rad = deg * DEG2RAD;

	v.y -= origin.y;
	v.z -= origin.z;

	v.y = (v.y * cos(rad)) - (v.z * sin(rad));
	v.z = (v.y * sin(rad)) + (v.z * cos(rad));

	v.y += origin.y;
	v.z += origin.z;
}

// distancia de manhattan = euclideana pero sin usar raiz cuadrada
// ideal si lo que queremos comprobar es UNICAMENTE si una distancia
// es mayor a otra (nos ahorramos raiz cuadrada)

constexpr inline float manhattan(const vec2& a, const vec2& b) {
	return length2(b - a);
}

constexpr inline float manhattan(const vec3& a, const vec3& b) {
	return length2(b - a);
}

constexpr inline float distance(const vec2& a, const vec2& b) {
	return length(b - a);
}

constexpr inline float distance(const vec3& a, const vec3& b) {
	return length(b - a);
}

constexpr inline vec3 projectVectorUV(const vec3& u, const vec3& v) {
	return (dot(u,v) / length2(v)) * v;
}

inline float triangleArea(const vec3& a, const vec3& b, const vec3& c) {
	//std::cout << a.x << " " << a.z << "   " << b.x << " " << b.z << "   " << c.x << " " << c.z << "   " << "\n";
	return std::abs((a.x * (b.z - c.z) + b.x * (c.z - a.z) + c.x * (a.z - b.z)) / 2.f);
}

// TODO: precalcular cosas necesarias y recurrentes