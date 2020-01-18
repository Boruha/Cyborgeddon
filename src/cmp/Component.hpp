#pragma once

#include <util/Alias.hpp>

struct Component {
	explicit Component(const EntityType type, const EntityID e_ID) : entityID(e_ID), entityType(type) {  }

	explicit operator bool() const { return entityType != UNDEFINED; }

	[[nodiscard]] const ComponentID& getID() const { return ID; }
	[[nodiscard]] const EntityID& getEntityID() const { return entityID; }
	[[nodiscard]] const EntityType& getEntityType() const { return entityType; }

	void makeUndefined() { entityType = UNDEFINED; } // si el tipo de entidad de un componente es undefined lo ignoramos

	~Component() = default;

	private:
		ComponentID ID { nextID++ };	// intentar hacer const
		inline static ComponentID nextID { 0 };

	protected:
		EntityID entityID; // intentar hacer const
		EntityType entityType;
		std::ostream& print(std::ostream& os, const std::string& name) const;
};