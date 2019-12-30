#pragma once

#include <cstdlib>
#include <util/Alias.hpp>

struct Component {
	explicit Component(const EntityType& type, const std::size_t& e_ID) : entityType(type), entityID(e_ID) {  }

	[[nodiscard]] const std::size_t& getID() const { return ID; }
	[[nodiscard]] const std::size_t& getEntityID() const { return entityID; }
	[[nodiscard]] const EntityType& getEntityType() const { return entityType; }

	protected:
		const EntityType entityType;
		const std::size_t entityID;

	private:
		const std::size_t ID { nextID++ };

		inline static std::size_t nextID { 0 };
};