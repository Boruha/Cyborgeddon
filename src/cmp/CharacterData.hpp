#pragma once

#include <cmp/Component.hpp>

struct CharacterData : Component {
	explicit CharacterData(const EntityType e_type, const std::size_t e_ID, const ModeType mode, const float health, const float attackDamage, const float attackingCooldown, const float switchingCooldown)
			: Component(e_type, e_ID), mode(mode), health(health), attackDamage(attackDamage), attackingCooldown(attackingCooldown), switchingCooldown(switchingCooldown) {  }

	friend std::ostream& operator<<(std::ostream& os, const CharacterData& data);

	ModeType	mode						{ NEUTRAL };
	bool 		attacking 					{  false  };
	bool		switchingMode				{  false  };
	float  		health    					{  100.f  };
	float 		attackDamage				{   50.f  };
	float 		attackingCooldown			{    0.f  };
	float  		currentAttackingCooldown   	{	 0.f  };
	float 		switchingCooldown			{    0.f  };
	float		currentSwitchingCooldown	{    0.f  };
};