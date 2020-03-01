#pragma once

#include <memory>
#include <cstdint>

enum AI_State : uint8_t
{
	PATROL_STATE,
	PURSUE_STATE,
	ATTACK_STATE,
	END_STATE
}; // END_STATE debe estar S I E M P R E al final

enum ColliderType : uint8_t
{
	DYNAMIC,
	STATIC,
	RAY,
	END_TYPE
}; // END_TYPE debe estar S I E M P R E al final

enum ModeType : uint8_t
{
	DEMON,
	ANGEL,
	NEUTRAL
};

enum EntityType : uint16_t
{
	UNDEFINED,
	PLAYER,
	CAMERA,
	ENEMY,
	KEY,
	DOOR,
	BULLET,
	FLOOR,
	WALL,
	NAV
};

using EntityID = uint16_t;
using ComponentID = uint16_t;