#pragma once

#include <cmp/Component.hpp>

struct CharacterData : Component {
	explicit CharacterData(const EntityType, const EntityID, const ModeType, const float hp,
							const float sw_cd, const float dmg, const float dmg_cd,
							const float dmg_rg, const float dsh, const float dsh_cd);

	[[nodiscard]] constexpr static std::string_view getName() {
		return "CharacterData";
	}

	friend std::ostream& operator<<(std::ostream& os, const CharacterData& data);

	ModeType	mode						{ NEUTRAL };
	bool 		attacking 					{  false  };
	bool		switchingMode				{  false  };
	bool 		dashing						{  false  };

	float  		health    					{  100.f  };
	float 		switchingCooldown			{    0.f  };
	float		currentSwitchingCooldown	{    0.f  };

	float 		attackDamage				{   50.f  };
	float 		attackingCooldown			{    0.f  };
	float  		currentAttackingCooldown   	{	 0.f  };
	float 		attackRange 				{    0.f  }; //NEW

	float 		dashSpeed					{    0.f  };
	float 		dashingCooldown				{	 0.f  };
	float		currentDashingCooldown		{    0.f  };

	// TODO : Componentes
	// 		Character 	-> mode, health, attack, attacking, attackingCooldown, currentAttackingCooldown, attackRange
	//		Switch		-> switching, switchingCooldown, currentSwitchingCooldown
	//		Dash		-> dashing, dashingCooldown, currentDashingCooldown
};