#include <sys/HighSpeedCollisionSystem.hpp>

void HighSpeedCollisionSystem::update(const std::unique_ptr<GameContext>& context) const {
	for (auto& fastObject : context->getRayBoundingComponents()) {
		if (fastObject.getEntityType() != UNDEFINED && fastObject.getEntityType() != PLAYER) {

			const Vector3f lastPos = *fastObject.pos;
			const Vector3f newPos  = lastPos + *fastObject.velocity;

			float lessDistance {-1};		// sabremos si la bala choca con algo porque la distancia siempre es positiva
			std::size_t closerEntity {0};
			bool killEntity {false};

			for (auto& collider : context->getBoundingComponents()) {
				if (collider.getEntityType() != UNDEFINED && collider.getEntityType() != PLAYER && !collider.passable) {
					if (intersects(lastPos, newPos, collider)) {
						float distance = (((collider.min + collider.max) / 2) - lastPos).length();

						if (lessDistance < 0 || (distance < lessDistance)) {
							killEntity = collider.getEntityType() == EntityType::ENEMY;
							lessDistance = distance;
							closerEntity = collider.getEntityID();
						}
					}
				}
			}

			if (lessDistance >= 0) { 								// si hemos chocado con algo
				if (killEntity)										// y ese algo se puede eliminar
					context->addToDestroy(closerEntity); 			// lo eliminamos
				context->addToDestroy(fastObject.getEntityID());	// y eliminamos el objeto rapido
			}
		}
	}
}

// return -> c pertenece al segmento ab o no
bool HighSpeedCollisionSystem::pointOnSegment(const Vector3f& a, const Vector3f& b, const Vector3f& c) const
{
	return c.x <= fmax(a.x, b.x) && c.x >= fmin(a.x, b.x) && c.z <= fmax(a.z, b.z) && c.z >= fmin(a.z, b.z);
}

// return 0  ->  abc forman una recta
// return 1  ->  abc se orientan a reloj
// return -1 ->  abc se orientan a contrarreloj
int HighSpeedCollisionSystem::segmentOrientation(const Vector3f& a, const Vector3f& b, const Vector3f& c) const
{
	float o = (b.z - a.z) * (c.x - b.x) - (b.x - a.x) * (c.z - b.z);

	return (o == 0) ? 0 : ((o > 0) ? 1 : -1);
}

// return -> ab intersecta a cd o no
bool HighSpeedCollisionSystem::intersects(const Vector3f& a, const Vector3f& b, const Vector3f& c, const Vector3f& d) const
{
	// orientaciones de la combinacion de los puntos de los dos segmentos
	int o1 = segmentOrientation(a, b, c);
	int o2 = segmentOrientation(a, b, d);
	int o3 = segmentOrientation(c, d, b);
	int o4 = segmentOrientation(c, d, a);

	// caso general
	if (o1 != o2 && o3 != o4)
		return true;

	// casos especiales
	return (o1 == 0 && pointOnSegment(a, b, c)) || (o2 == 0 && pointOnSegment(a, b, d)) ||
		   (o3 == 0 && pointOnSegment(c, d, b)) || (o4 == 0 && pointOnSegment(c, d, a));
}

bool HighSpeedCollisionSystem::intersects(const Vector3f& lastPos, const Vector3f& newPos, const BoundingBox& box) const
{
	return 	intersects(lastPos, newPos, box.min, Vector3f(box.min.x, 0, box.max.z)) ||
			intersects(lastPos, newPos, Vector3f(box.min.x, 0, box.max.z), box.max) ||
			intersects(lastPos, newPos, box.max, Vector3f(box.max.x, 0, box.min.z)) ||
			intersects(lastPos, newPos, Vector3f(box.max.x, 0, box.min.z), box.min);
}