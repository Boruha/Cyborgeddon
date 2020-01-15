#pragma once

#include <util/Alias.hpp>

struct Component {
	explicit Component(const EntityType type, const EntityID e_ID) : entityID(e_ID), entityType(type) {  }

	[[nodiscard]] const ComponentID& getID() const { return ID; }
	[[nodiscard]] const EntityID& getEntityID() const { return entityID; }
	[[nodiscard]] const EntityType& getEntityType() const { return entityType; }

	void makeUndefined() { entityType = UNDEFINED; } // si el tipo de entidad de un componente es undefined lo ignoramos

	~Component() = default;

	private:
		ComponentID ID { nextID++ };
		inline static ComponentID nextID { 0 };

	protected:
		EntityID entityID;
		EntityType entityType;
		std::ostream& print(std::ostream& os, const std::string& name) const;
};