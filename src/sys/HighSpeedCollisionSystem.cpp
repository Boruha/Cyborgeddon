#include <sys/HighSpeedCollisionSystem.hpp>

struct EntityHitData {
	float lessDistance { -1 };		// sabremos si la bala choca con algo porque la distancia siempre es positiva
	std::size_t closerEntity { 0 };
	bool damageEntity { false };
};

void HighSpeedCollisionSystem::update(const std::unique_ptr<GameContext> &context, const float deltaTime) const {
	for (const auto& fastObject : context->getPhysicsComponents()) {
		if (fastObject.getEntityType() == BULLET) {

			EntityHitData hitData;

			Vector3f lastPos = fastObject.position;
			Vector3f newPos	 = fastObject.position + fastObject.velocity * deltaTime;

			for (const auto& staticCollider : context->getStaticBoundingComponents())
				checkHit(lastPos, newPos, staticCollider, hitData);

			for (const auto& otherCollider : context->getBoundingComponents())
				checkHit(lastPos, newPos, otherCollider, hitData);

			if (hitData.lessDistance >= 0) { // si hemos chocado con algo
				if (hitData.damageEntity) {
					auto bullet 		 = context->getEntityByID(fastObject.getEntityID());
					auto entityToDamage = context->getEntityByID(hitData.closerEntity);

					damageEntity(*bullet->bulletData, *entityToDamage->characterData); // lo dañamos
				}
				context->addToDestroy(fastObject.getEntityID());            // y eliminamos el objeto rapido
			}
		}
	}
}

void HighSpeedCollisionSystem::checkHit(const Vector3f& lastPos, const Vector3f& newPos, const BoundingBox& box, EntityHitData& hitData) const {
	if (box.getEntityType() != UNDEFINED && !box.passable) {
		if (intersects(lastPos, newPos, box)) {
			float distance = (((box.min + box.max) / 2) - lastPos).length();

			if (hitData.lessDistance < 0 || (distance < hitData.lessDistance)) {
				hitData.damageEntity = box.getEntityType() == ENEMY;			// de momento matamos enemigos
				hitData.lessDistance = distance;								// si son lo mas cercano
				hitData.closerEntity = box.getEntityID();						// aqui guardamos el id por si necesitamos borrar
			}
		}
	}
}

void HighSpeedCollisionSystem::damageEntity(const BulletData& bullet, CharacterData& character) const {
	if (bullet.damageType != character.mode) {
		character.health -= (bullet.damage * 2);
	} else {
		character.health -= bullet.damage;
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