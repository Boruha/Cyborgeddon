#include <ent/Entity.hpp>

#include <cmp/Components.hpp>

Entity::Entity(EntityType type) : type(type), ID(++nextID) {  }

void undefine(Component * cmp) {
	if (cmp) {
		cmp->makeUndefined();
		cmp = nullptr;
	}
}

void Entity::makeUndefined() {
	type = UNDEFINED;

	undefine(transformable);
	undefine(velocity);
	undefine(physics);
	undefine(bulletData);
	undefine(characterData);
	undefine(collider);
	undefine(ai);
	undefine(triggStaticAABB);
	undefine(triggerMovSphere);
	undefine(rigidStaticAABB);

    inode = nullptr;
}