#pragma once

#include <cmp/Component.hpp>

struct CharacterData : Component {
	explicit CharacterData(const EntityType e_type, const std::size_t e_ID, const bool mode, const float health, const float attackDamage, const float attackingCooldown)
			: Component(e_type, e_ID), mode(mode), health(health), attackDamage(attackDamage), attackingCooldown(attackingCooldown) {  }

	friend std::ostream& operator<<(std::ostream& os, const CharacterData& data);

	bool 		mode 	   					{ false };		// true(1) = angel, false(0) = demon;
	bool 		attacking 					{ false };
	float  		health    					{ 100.f };
	const float attackDamage				{  50.f };
	const float attackingCooldown			{   0.f };
	float  		currentAttackingCooldown   	{	0.f };
};