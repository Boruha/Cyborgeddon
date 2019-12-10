#pragma once

#include <zconf.h>

enum EntityID : u_int8_t { PLAYER_ID, CAMERA_ID, KEY_ID, DOOR_ID };

struct Entity
{
	explicit Entity(EntityID ID) : ID(ID){}
	[[nodiscard]] const EntityID& getID() const { return ID; }
protected:
	EntityID ID;
};