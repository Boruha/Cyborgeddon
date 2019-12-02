#pragma once

#include <irrlicht/irrlicht.h>
#include <cmp/EventReceiver.hpp>
#include <ent/EntityPlayer.hpp>

#include <vector>

struct InputSystem
{
	InputSystem() = default;

	int init(irr::IrrlichtDevice*);
	void update(EntityPlayer&);

	EventReceiver eventReceiver;
};