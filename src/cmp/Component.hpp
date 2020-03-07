#pragma once

#include <util/Alias.hpp>

#include <glm/glm.hpp>

#include <string_view>

using glm::vec3; // Component no lo usa pero casi todos los componentes si

struct Component {
	explicit Component(const EntityType type, const EntityID e_ID) : entityID(e_ID), entityType(type) {  }

	explicit operator bool() const { return entityType != UNDEFINED; }

	inline static void resetIDManagementValue() { nextID = 0; }

	[[nodiscard]] const ComponentID& getID() const { return ID; }
	[[nodiscard]] const EntityID& getEntityID() const { return entityID; }
	[[nodiscard]] const EntityType& getEntityType() const { return entityType; }
	[[nodiscard]] constexpr virtual std::string_view getName() const = 0;

	template <typename T>
	static std::size_t getCmpTypeID() {
		static std::size_t typeID { nextCmpTypeID++ };
		return typeID;
	}

	void destroy() { entityType = UNDEFINED; } // si el tipo de entidad de un componente es undefined lo ignoramos

	~Component() = default;

	private:
		ComponentID ID { nextID++ };	// intentar hacer const
		inline static ComponentID nextID { 0 };
		inline static std::size_t nextCmpTypeID { 0 };

	protected:
		EntityID entityID; // intentar hacer const
		EntityType entityType;
		std::ostream& print(std::ostream& os, std::string_view name) const;
};