#pragma once

#include <Engine/util/Plane.hpp>
#include <Engine/util/Line.hpp>
#include <Engine/util/Math.hpp>

// punto de interseccion entre una recta y un plano
constexpr inline vec3 intersectionPoint(const Plane& p, const Line& l) {
    return l.a + ((l.b - l.a) * ((p.distance - dot(p.normal, l.a)) / dot(p.normal, (l.b - l.a))));
}

// return -> p pertenece al segmento
inline bool segmentContainsPointXZ(const Line& segment, const vec3& p)
{
	return p.x <= fmax(segment.a.x, segment.b.x) && p.x >= fmin(segment.a.x, segment.b.x) && p.z <= fmax(segment.a.z, segment.b.z) && p.z >= fmin(segment.a.z, segment.b.z);
}

inline float segmentDistancePointXZ(const vec3& r, const vec3& ecLine) //NEW
{
	float rInLine = (ecLine.x * r.x) + (ecLine.y * r.z) + ecLine.z;
	if(rInLine < 0) rInLine = -rInLine;

	return rInLine / std::sqrt((ecLine.x * ecLine.x) + (ecLine.y * ecLine.y));
}

//Ax + By + C = 0  ||  vX = A, vY = B, vZ = C
constexpr inline vec3 lineEcByTwoPoints(const vec3& p1, const vec3& vDir) //NEW
{
	return vec3(vDir.z, -vDir.x, (-p1.x * vDir.z) + (p1.z * vDir.x));
}

enum LinePointOrientation { ALIGNED, CLOCKWISE, ANTICLOCKWISE };

inline LinePointOrientation linePointOrientationXZ(const Line& r, const vec3& p)
{
	const float o = (r.b.z - r.a.z) * (p.x - r.b.x) - (r.b.x - r.a.x) * (p.z - r.b.z);

	return (o == 0) ? ALIGNED : ((o > 0) ? CLOCKWISE : ANTICLOCKWISE);
}

inline bool lineIntersection(const Line& r, const Line& s)
{
	// orientaciones de la combinacion de los puntos de los dos segmentos
	const int o1 = linePointOrientationXZ(r, s.a);
	const int o2 = linePointOrientationXZ(r, s.b);
	const int o3 = linePointOrientationXZ(s, r.b);
	const int o4 = linePointOrientationXZ(s, r.a);

	// caso general
	if (o1 != o2 && o3 != o4)
		return true;

	// casos especiales
	return (o1 == 0 && segmentContainsPointXZ(r, s.a)) || (o2 == 0 && segmentContainsPointXZ(r, s.b)) ||
		   (o3 == 0 && segmentContainsPointXZ(s, r.b)) || (o4 == 0 && segmentContainsPointXZ(s, r.a));
}

inline bool SphereWillIntersectTOBB(const float rad, const vec3& vertex, const vec3& ecLine) //NEW
{
	return rad > segmentDistancePointXZ(vertex, ecLine);

}

inline bool lineAABBIntersectionXZ(const Line& r, const vec3& aabbMin, const vec3& aabbMax)
{
	return lineIntersection(r, Line(aabbMin, vec3(aabbMin.x, 0, aabbMax.z))) ||
		   lineIntersection(r, Line(vec3(aabbMin.x, 0, aabbMax.z), aabbMax)) ||
		   lineIntersection(r, Line(aabbMax, vec3(aabbMax.x, 0, aabbMin.z))) ||
		   lineIntersection(r, Line(vec3(aabbMax.x, 0, aabbMin.z), aabbMin));
}

inline bool intersectionAABB(const vec3& min1,  const vec3& max1, const vec3& min2,  const vec3& max2) {
    for(unsigned i = 0; i < 3; i+=2)
        if (!(greater_e(max1[i], min2[i]) && less_e(min1[i], max2[i])))
           return false;

    return true;
}

inline bool intersectionSphereAABB(const vec3& pos, const float radius, const vec3& min, const vec3& max) {
	float squareDistance = 0.0f;

	for( int i = 0; i < 3; i+=2 ){
		if( pos[i] < min[i] )
			squareDistance += (min[i] - pos[i]) * (min[i] - pos[i]);
		if( pos[i] > max[i] )
			squareDistance += (pos[i] - max[i]) * (pos[i] - max[i]);
	}

	return greater_e(radius * radius, squareDistance);
}

inline bool intersectionLineSphereXZ(const Line& line, const vec3& center, const float radius) {
	const glm::vec2 direction (line.b.x - line.a.x, line.b.z - line.a.z); // v director de la recta
	const glm::vec2 center2origin (line.a.x - center.x, line.a.z - center.z); // v centro esfera a un punto origen de la recta

	const float a = dot(direction, direction);
	const float b = 2 * dot(center2origin, direction);
	const float c = dot(center2origin, center2origin) - radius * radius;

	return (b * b) - (4 * a * c) >= 0; // si el discriminante de la ecuacion es menor que 0 NO hay colision
}

inline glm::vec3 getIntersectionPointXZ(const Line& AB, const Line& CD) {
	const float a1 = AB.b.z - AB.a.z;
	const float b1 = AB.a.x - AB.b.x;
	const float c1 = a1 * (AB.a.x) + b1 * (AB.a.z);

	const float a2 = CD.b.z - CD.a.z;
	const float b2 = CD.a.x - CD.b.x;
	const float c2 = a2 * (CD.a.x)+ b2 * (CD.a.z);

	const float det = a1 * b2 - a2 * b1;

	const float x = (b2 * c1 - b1 * c2) / det;
	const float z = (a1 * c2 - a2 * c1) / det;

	return vec3(x, 0, z);
}