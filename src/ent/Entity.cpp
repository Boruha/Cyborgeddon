#include <ent/Entity.hpp>

#include <cmp/Components.hpp>

Entity::Entity(EntityType type) : type(type), ID(nextID++) {

}

Entity::~Entity() = default;

void Entity::destroy() {
	type = UNDEFINED;

	for (const auto & pair : components)
		pair.second->destroy();

	components.clear();

	if (inode)
		inode->remove();

	inode = nullptr;
}
