#pragma once

#include <cmp/Component.hpp>

struct CharacterData : Component {
	explicit CharacterData(EntityType, EntityID, ModeType, float hp, float sw_cd, float dmg, float dmg_cd, float dsh, float dsh_cd);

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

	float 		dashSpeed					{    0.f  };
	float 		dashingCooldown				{	 0.f  };
	float		currentDashingCooldown		{    0.f  };

	// TODO : Componentes
	// 		Character 	-> mode, health, attack, attacking, attackingCooldown, currentAttackingCooldown
	//		Switch		-> switching, switchingCooldown, currentSwitchingCooldown
	//		Dash		-> dashing, dashingCooldown, currentDashingCooldown
};