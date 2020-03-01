#pragma once

#include <glm/glm.hpp>

using glm::vec2;
using glm::vec3;

constexpr double PI = 3.141592653589793238463;
constexpr double DEG2RAD = PI / 180.0;
constexpr double RAD2DEG = 180.0 / PI;

constexpr float  EPSILON = 0.001f;

inline bool equal_e(const float value, const float toCompare) { return std::abs(value - toCompare) < EPSILON; }
inline bool greater_e(const float value, const float toCompare) { return (value - toCompare) > EPSILON; }
inline bool less_e(const float value, const float toCompare) { return (value - toCompare) < -EPSILON; }

inline float nearestAngle(const float first, const float second) {
    return first + static_cast<float>(std::fmod((std::fmod((std::fmod(second - first, 360) + 540), 360) - 180), 360));
}

inline float length2(const vec2& v) {
	return (v.x * v.x) + (v.y * v.y);
}

inline float length2(const vec3& v) {
	return (v.x * v.x) + (v.y * v.y) + (v.z * v.z);
}

inline float length(const vec2& v) {
	return std::sqrt( length2(v) );
}

inline float length(const vec3& v) {
	return std::sqrt( length2(v) );
}

inline vec2& normalize(vec2& v) {
	const float l = length(v);

	if (l != 0)
		v /= l;

	return v;
}

inline vec3& normalize(vec3& v) {
	const float l = length(v);

	if (l != 0)
		v /= l;

	return v;
}

inline float dot(const vec2& v1, const vec2& v2) {
	return v1.x * v2.x + v1.y * v2.y;
}

inline float dot(const vec3& v1, const vec3& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

inline float getRotationYfromXZ(const vec3& v) {
	return float(std::atan2(v.x, v.z) * RAD2DEG);
}

inline vec3 getXZfromRotationY(double deg) {
	deg *= DEG2RAD;

	const auto x = float(sin(deg));
	const auto z = float(cos(deg));

	return vec3(x,0,z);
}

inline void abs(vec2& v) {
	v.x = std::abs(v.x);
	v.y = std::abs(v.y);
}

inline void abs(vec3& v) {
	v.x = std::abs(v.x);
	v.y = std::abs(v.y);
	v.z = std::abs(v.z);
}

inline void rotateXZ(vec3& v, double deg, const vec3& origin = vec3()) {
	deg *= DEG2RAD;

	v.x -= origin.x;
	v.z -= origin.z;

	v.x = v.x * cos(deg) - v.z * sin(deg);
	v.z = v.x * sin(deg) + v.z * cos(deg);

	v.x += origin.x;
	v.z += origin.z;
}

inline void rotateXY(vec3& v, double deg, const vec3& origin = vec3()) {
	deg *= DEG2RAD;

	v.x -= origin.x;
	v.y -= origin.y;

	v.x = v.x * cos(deg) - v.y * sin(deg);
	v.y = v.x * sin(deg) + v.y * cos(deg);

	v.x += origin.x;
	v.y += origin.y;
}

inline void rotateYZ(vec3& v, double deg, const vec3& origin = vec3()) {
	deg *= DEG2RAD;

	v.y -= origin.y;
	v.z -= origin.z;

	v.y = v.y * cos(deg) - v.z * sin(deg);
	v.z = v.y * sin(deg) + v.z * cos(deg);

	v.y += origin.y;
	v.z += origin.z;
}

// distancia de manhattan = euclideana pero sin usar raiz cuadrada
// ideal si lo que queremos comprobar es UNICAMENTE si una distancia
// es mayor a otra (nos ahorramos raiz cuadrada)

inline float manhattan(const vec2& a, const vec2& b) {
	return length2(b - a);
}

inline float manhattan(const vec3& a, const vec3& b) {
	return length2(b - a);
}

inline float distance(const vec2& a, const vec2& b) {
	return length(b - a);
}

inline float distance(const vec3& a, const vec3& b) {
	return length(b - a);
}
// TODO: precalcular cosas necesarias y recurrentes