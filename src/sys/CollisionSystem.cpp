#include <sys/CollisionSystem.hpp>
#include <Engine/util/MathIntersection.hpp>
#include <glm/gtx/string_cast.hpp>

//  TODO : PROYECCION AB SOBRE AD, OBTENER D, Y COMPROBAR SI LA DISTANCIA CD ES MENOR QUE EL RADIO
// https://stackoverflow.com/questions/1073336/circle-line-segment-collision-detection-algorithm


bool intersectCircleSegment(const vec3& a, const vec3& b, const vec3& center, const float radius) {
	const vec3 ab = b - a;
	const vec3 ac = center - a;

	const vec3 projection = a + (ab * (dot(ac, ab) / dot(ab, ab)));

	std::cout << "\nProjection: " << glm::to_string(projection) << "\n";

	return less_e(distance(projection, center), radius);
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
					if (obb && intersectionAABB(bounding.min, bounding.max, obb.min, obb.max)) {
						for (unsigned j = 0; j < 3; ++j) {
							if (intersectCircleSegment(obb.vertices[j % 3], obb.vertices[(j + 1) % 3], bounding.center, bounding.radius)) {

								std::cout   << "\nBounding con centro " << glm::to_string(bounding.center)
											<< " y radio " << bounding.radius
											<< " ha colisionado con el segmento " << glm::to_string(obb.vertices[j % 3])
											<< " --- " << glm::to_string(obb.vertices[(j + 1) % 3]) << "\n";

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
	}
}