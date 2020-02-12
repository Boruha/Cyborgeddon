#include <sys/HighSpeedCollisionSystem.hpp>
#include <util/ComponentConstants.hpp>
#include <Engine/util/MathIntersection.hpp>
#include <iostream>

struct EntityHitData {
	float lessDistance { -1 };		// sabremos si la bala choca con algo porque la distancia siempre es positiva
	EntityID closerEntity { 0 };
	bool damageEntity { false };
};

void HighSpeedCollisionSystem::update(const std::unique_ptr<GameContext> &context, const float deltaTime) const {
	std::array<EntityID, 8> bulletsToDestroy { }; // suponemos que como mucho pueden morir en una iteracion 8 bullets (que creo que ya es mucho y no habra problema)
	unsigned numToDestroy = 0;

	for (const auto& fastObject : std::get<vector<Physics>>(context->getComponents(PHYSICS_TYPE))) {
		if (fastObject.getEntityType() == BULLET) {

			EntityHitData hitData;

			const Line bulletRay(fastObject.position, fastObject.position + fastObject.velocity * deltaTime);

			for (const auto& staticCollider : std::get<vector<BoundingBox>>(context->getComponents(STATIC_BOUNDING_BOX_TYPE)))
				checkHit(bulletRay, staticCollider, hitData);

			for (const auto& otherCollider : std::get<vector<BoundingBox>>(context->getComponents(SPECIAL_BOUNDING_BOX_TYPE)))
				checkHit(bulletRay, otherCollider, hitData);

			if (!less_e(hitData.lessDistance, 0)) { // si hemos chocado con algo
				if (hitData.damageEntity) {
					const auto bullet 			= context->getEntityByID(fastObject.getEntityID());
					const auto entityToDamage 	= context->getEntityByID(hitData.closerEntity);

					damageEntity(*bullet.bulletData, *entityToDamage.characterData); // lo dañamos
				}

				bulletsToDestroy[numToDestroy++] = fastObject.getEntityID();
			}
		}
	}

	for (unsigned i = 0; i < numToDestroy; ++i)
		context->addToDestroy(bulletsToDestroy[i]);
}

void HighSpeedCollisionSystem::checkHit(const Line& bulletRay, const BoundingBox& box, EntityHitData& hitData) const {
	if (box && !box.passable) {
		if (lineAABBIntersectionXZ(bulletRay, box.min, box.max)) {
			const float distance = length(((box.min + box.max) / 2.f) - bulletRay.a); // distancia desde el origen de la bala hasta el objeto (bullet.a -> posInicial, bullet.b -> posFinal)

			if (less_e(hitData.lessDistance, 0) || (less_e(distance, hitData.lessDistance))) {
				hitData.damageEntity = box.getEntityType() == ENEMY;			// de momento matamos enemigos
				hitData.lessDistance = distance;								// si son lo mas cercano
				hitData.closerEntity = box.getEntityID();						// aqui guardamos el id por si necesitamos borrar
			}
		}
	}
}

void HighSpeedCollisionSystem::damageEntity(const BulletData& bullet, CharacterData& character) const {
	if (character.mode == NEUTRAL) {
		character.health -= bullet.damage;
	} else {
		if (character.mode != bullet.damageType)
			character.health -= (bullet.damage * FACTOR_DIFFERENT_MODE);
		else
			character.health -= (bullet.damage * FACTOR_SAME_MODE);
	}
}