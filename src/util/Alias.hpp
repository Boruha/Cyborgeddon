#pragma once

#include <cstdint>

enum ModeType : uint8_t
{
	DEMON   = 0,
	ANGEL   = 1,
	NEUTRAL = 2
};

enum EntityType : uint16_t
{
	UNDEFINED = 0,
	PLAYER    = 1,
	CAMERA    = 2,
	ENEMY     = 3,
	KEY       = 4,
	DOOR      = 5,
	BULLET    = 6,
	FLOOR     = 7,
	WALL      = 8,
	NAV       = 9,
	LIGHT     = 10,
	VIDEO     = 11,
	COLLIDER  = 12,
	TEXTURE   = 13,
	MENU_OPTIONS = 14
};

enum StateEnum : uint8_t
{
	NO_STATE,
	INIT,
	TUTORIAL,
	INGAME,
	PAUSE,
	ENDING
};

using EntityID = uint16_t;
using ComponentID = uint16_t;