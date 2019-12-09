#pragma once

#include <cstdint>

enum EntityID : u_int8_t { NULL_ID, PLAYER_ID, CAMERA_ID };

struct Entity
{
	Entity() = default;
	explicit Entity(EntityID ID) : ID(ID){}
	virtual ~Entity() = default;

	[[nodiscard]] const EntityID& getID() const { return ID; }

	protected:
		EntityID ID { NULL_ID };
};