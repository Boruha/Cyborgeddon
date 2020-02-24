#pragma once

#include <Engine/util/Plane.hpp>
#include <Engine/util/Line.hpp>
#include <Engine/util/Math.hpp>

// punto de interseccion entre una recta y un plano
inline vec3 intersectionPoint(const Plane& p, const Line& l) {
    return l.a + ((l.b - l.a) * ((p.distance - dot(p.normal, l.a)) / dot(p.normal, (l.b - l.a))));
}


// return -> p pertenece al segmento
inline bool segmentContainsPointXZ(const Line& segment, const vec3& p)
{
	return p.x <= fmax(segment.a.x, segment.b.x) && p.x >= fmin(segment.a.x, segment.b.x) && p.z <= fmax(segment.a.z, segment.b.z) && p.z >= fmin(segment.a.z, segment.b.z);
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