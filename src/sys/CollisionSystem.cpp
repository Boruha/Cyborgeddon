#include <sys/CollisionSystem.hpp>
#include <Engine/util/MathIntersection.hpp>
#include <glm/gtx/string_cast.hpp>

//  TODO : PROYECCION AB SOBRE AD, OBTENER D, Y COMPROBAR SI LA DISTANCIA CD ES MENOR QUE EL RADIO
// https://stackoverflow.com/questions/1073336/circle-line-segment-collision-detection-algorithm


bool segmentCutsCircle(const vec3& a, const vec3& b, const vec3& center, const float radius) {
	const vec3 ab = b - a;
	const vec3 ac = center - a;

	const vec3 projection = a + (ab * (dot(ac, ab) / dot(ab, ab)));

	std::cout << "\nProjection: " << glm::to_string(projection) << "\n";

	return segmentContainsPointXZ(Line(a, b), projection) && less_e(manhattan(projection, center), radius * radius);
}

bool intersectionCircleTriangle(const CircleBounding& circle, const TriangleOBB& triangle) {

	const std::array<vec3, 3>& v = triangle.vertices;

	const std::array<Line, 3> seg  {
		Line(v[0], v[1]),
		Line(v[1], v[2]),
		Line(v[1], v[0])
	};

	const std::array<vec3, 3> proj {};

	for (unsigned i = 0; i < 3; ++i)
		const_cast<vec3&>(proj[i]) = seg[i].a + ((seg[i].b - seg[i].a) * (dot(circle.center - seg[i].a, seg[i].b - seg[i].a) / dot(seg[i].b - seg[i].a, seg[i].b - seg[i].a)));

	// si algun segmento corta al circulo
	for (unsigned i = 0; i < 3; ++i)
		if (segmentContainsPointXZ(seg[i], proj[i]) && less_e(manhattan(proj[i], circle.center), circle.radius * circle.radius)) {
			std::cout << "Segmento " << glm::to_string(seg[i].a) << " --- " << glm::to_string(seg[i].b) << " contiene a punto " << glm::to_string(proj[i]) << " del circulo con centro en " << glm::to_string(circle.center) << " y radio " << circle.radius << "\n";
			return true;
		}

	// si el triangulo esta completamente dentro del circulo
	bool triangleInsideCircle = true;

	for (unsigned i = 0; i < 3; ++i)
		triangleInsideCircle &= less_e(manhattan(proj[i], circle.center), circle.radius * circle.radius);

	if (triangleInsideCircle) {
		std::cout << "El triangulo " << triangle << " esta completamente dentro del circulo " << circle << "\n";
		return true;
	}

	// si el circulo esta completamente dentro del triangulo
	bool distanceProjGreaterThanRadius = true;

	for (unsigned i = 0; i < 3; ++i)
		distanceProjGreaterThanRadius &= greater_e(manhattan(proj[i], circle.center), circle.radius * circle.radius);

	std::array<float, 3> area {};

	for (unsigned i = 0; i < 3; ++i)
		area[i] = triangleArea(v[i], v[(i + 1) % 3], circle.center);

	const float area_of_triangle = triangleArea(v[0], v[1], v[2]);

	std::cout << "\nArea del triangulo " << triangle << " \n" << area_of_triangle << "\n";
	std::cout << "Areas de los triangulos formados por la vaina " << area[0] << " " << area[1] << " " << area[2] << "\n";
	std::cout << "Suma: " << area[0] + area[1] + area[2] << "\n";
	std::cout << "Circle position " << glm::to_string(circle.center) << "\n";

	if (equal_e(area_of_triangle, area[0] + area[1] + area[2]))
		return true;

/*
	for (unsigned i = 0; i < 3; ++i) {
		if (segmentCutsCircle(triangle.vertices[i % 3], triangle.vertices[(i + 1) % 3], circle.center, circle.radius)) {

			std::cout   << "\nBounding con centro " << glm::to_string(circle.center)
			            << " y radio " << circle.radius
			            << " ha colisionado con el segmento " << glm::to_string(triangle.vertices[i % 3])
			            << " --- " << glm::to_string(triangle.vertices[(i + 1) % 3]) << "\n";

			return true;
		}
	}
*/
	return false;
}

bool triangleInCircle(const CircleBounding& circle, const TriangleOBB& triangle) {
	return less_e(manhattan(triangle.vertices[0], circle.center), circle.radius * circle.radius)
	       &&      less_e(manhattan(triangle.vertices[1], circle.center), circle.radius * circle.radius)
	       &&      less_e(manhattan(triangle.vertices[2], circle.center), circle.radius * circle.radius);
}

bool circleInTriangle(const CircleBounding& circle, const TriangleOBB& triangle) {

}

bool intersectCircleTriangle(const CircleBounding& circle, const TriangleOBB& triangle) {
	return intersectionAABB(circle.min, circle.max, triangle.min, triangle.max) && intersectionCircleTriangle(circle, triangle);
}

void CollisionSystem::fixedUpdate(const Context &context, const float deltaTime) {

	for (auto & bounding : context->getComponents().getComponents<CircleBounding>()) {
		if (bounding && bounding.getEntityType() == PLAYER) {

			vec3& velocity = *bounding.velocity;

			for (unsigned i = 0; i < 3; i+=2) {
				if (equal_e(velocity[i], 0))
					continue;

				bounding.min[i] += velocity[i];
				bounding.max[i] += velocity[i];

				bounding.center[i] += velocity[i];

				for (const auto & obb : context->getComponents().getComponents<TriangleOBB>()) {
					if (obb && intersectCircleTriangle(bounding, obb)) {
						bounding.center[i] -= velocity[i];

						bounding.max[i] -= velocity[i];
						bounding.min[i] -= velocity[i];

						velocity[i] = 0.f;

						break;
					}
				}
			}
		}
	}
}