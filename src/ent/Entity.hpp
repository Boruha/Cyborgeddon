#pragma once

#include <cstdlib>

enum EntityID : std::size_t { NULL_ID, PLAYER_ID, CAMERA_ID, ENEMY_ID, KEY_ID, DOOR_ID, BULLET_ID, FLOOR_ID, WALL_ID };

struct Entity
{
	Entity() = default;
	explicit Entity(EntityID ID) : ID(ID) {  }

	[[nodiscard]] const EntityID& getID() const { return ID; }

	protected:
		EntityID ID { NULL_ID };
};

// TODO: generalizar entidades

