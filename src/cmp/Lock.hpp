#pragma once

#include <cmp/Component.hpp>
#include <ent/Entity.hpp>
#include <array>

struct Lock : Component
{
    explicit Lock(EntityType, EntityID);

	[[nodiscard]] constexpr static std::string_view getName() {
		return "Lock Data";
	}
    
    std::array<EntityID, 2> keys;
    std::array<bool, 2>     checks { false, false };	
};