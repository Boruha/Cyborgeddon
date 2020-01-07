#pragma once

#include <cmp/Component.hpp>

struct CharacterData : Component {
	explicit CharacterData(const EntityType e_type, const std::size_t e_ID, const bool mode, const int health, const int attackingCooldown)
			: Component(e_type, e_ID), mode(mode), health(health), attackingCooldown(attackingCooldown) {  }

	bool attacking 			{ false };
	bool mode 	   			{ false };		// true(1) = angel, false(0) = demon;
	int  health    			{  100  };
	int  attackingCooldown  {   0   };
};