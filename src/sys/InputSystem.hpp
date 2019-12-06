#pragma once

#include <irrlicht/irrlicht.h>
#include <ent/Entities.hpp>

#include <vector>
#include <util/EventReceiver.hpp>

struct InputSystem
{
	InputSystem() = default;

	int init(irr::IrrlichtDevice* device);
	void update(EntityPlayer& player);

	private:
		EventReceiver eventReceiver;

		static void w_pressed(EntityPlayer& player) { ++player.velocity.velocity.z; }
		static void a_pressed(EntityPlayer& player) { --player.velocity.velocity.x; }
		static void s_pressed(EntityPlayer& player) { --player.velocity.velocity.z; }
		static void d_pressed(EntityPlayer& player) { ++player.velocity.velocity.x; }

	struct TKey2func {
		irr::EKEY_CODE key;
		void (*p_func)(EntityPlayer&);
	};

	const TKey2func mapping[5]
	{
		{irr::KEY_KEY_W,                 		w_pressed },
		{irr::KEY_KEY_A,                 		a_pressed },
		{irr::KEY_KEY_S,                 		s_pressed },
		{irr::KEY_KEY_D,                 		d_pressed },
		{static_cast<irr::EKEY_CODE>(0), nullptr }
	};
};