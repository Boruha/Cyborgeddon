#include <ent/Entity.hpp>

#include <cmp/Transformable.hpp>
#include <cmp/Velocity.hpp>
#include <cmp/BoundingBox.hpp>
#include <cmp/AI.hpp>
#include <cmp/Physics.hpp>
#include <cmp/BulletData.hpp>
#include <cmp/CharacterData.hpp>

Entity::Entity(EntityType type) : type(type), ID(++nextID) {  }

void Entity::makeUndefined() {
	type = UNDEFINED;

	if (transformable)
	    transformable->makeUndefined();
    if (velocity)
        velocity->makeUndefined();
    if (physics)
        physics->makeUndefined();
    if (bulletData)
        bulletData->makeUndefined();
    if (characterData)
        characterData->makeUndefined();
    if (collider)
        collider->makeUndefined();
    if (ai)
        ai->makeUndefined();
	if (triggStaticAABB)
		triggStaticAABB->makeUndefined();
	if (rigidMovSphere)
		rigidMovSphere->makeUndefined();

    transformable 	= { nullptr };
    velocity 		= { nullptr };
    physics			= { nullptr };
    bulletData		= { nullptr };
    characterData	= { nullptr };
    collider		= { nullptr };
    ai				= { nullptr };
    triggStaticAABB = { nullptr };
	rigidMovSphere  = { nullptr };

    inode			= { nullptr };
}
