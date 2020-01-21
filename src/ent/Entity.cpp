#include <ent/Entity.hpp>

Entity::Entity(EntityType type) : type(type), ID(++nextID) {
	components.reserve(8);
}

void Entity::addComponent(Component& componente) {
	components.emplace_back(&componente);
}

void Entity::makeUndefined() {
	type = UNDEFINED;

	for (auto& component : components) {
		if (component)
			component->makeUndefined();
		component = nullptr;
	}

	components.clear();
	components.shrink_to_fit();
}
