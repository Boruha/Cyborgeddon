#pragma once

#include <cstdint>

enum AI_State : uint8_t
{
	PATROL_STATE = 0,
	PURSUE_STATE = 1,
	ATTACK_STATE = 2,
	END_STATE = 3
}; // END_STATE debe estar S I E M P R E al final

enum ModeType : uint8_t
{
	DEMON   = 0,
	ANGEL   = 1,
	NEUTRAL = 2
};

enum EntityType : uint16_t
{
	UNDEFINED = 0,
	PLAYER = 1,
	CAMERA = 2,
	ENEMY = 3,
	KEY = 4,
	DOOR = 5,
	BULLET = 6,
	FLOOR = 7,
	WALL = 8,
	NAV = 9
};

using EntityID = uint16_t;
using ComponentID = uint16_t;