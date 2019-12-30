#pragma once

#include <cstdlib>
#include <cmp/Alive.hpp>
#include <util/Alias.hpp>

struct Entity
{
	Entity() = default;
	explicit Entity(EntityType type) : type(type) {  }
	virtual ~Entity() = default;

	[[nodiscard]] const EntityType& getType() const { return type; }
	[[nodiscard]] const std::size_t& getID() const { return ID; }

	Alive alive { type, ID };

	protected:
		const EntityType type { UNDEFINED };
		const std::size_t ID { nextID++ };

	private:
		inline static std::size_t nextID { 0 };
};

// TODO: generalizar entidades

