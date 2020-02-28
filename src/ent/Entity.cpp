#include <ent/Entity.hpp>

#include <cmp/Components.hpp>

Entity::Entity(EntityType type) : type(type), ID(nextID++) {

}

Entity::~Entity() = default;

void Entity::destroy() {
	type = UNDEFINED;

	auto * render = getComponent<Render>();

	if (render)
		render->node->remove();

	for (const auto & pair : components)
		pair.second->destroy();

	components.clear();
}
