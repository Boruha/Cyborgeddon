#pragma once

#include <SunlightEngine/EventReceiver.hpp>
#include <SunlightEngine/KeyCodes.hpp>

#include <ent/Entities.hpp>

#include <vector>
#include <src/man/EntityManager.hpp>

struct InputSystem
{
	InputSystem() = default;

	int init(Sun::Device* device);
	void update(const std::vector<std::unique_ptr<EntityPlayer>>& players);

	private:
		Sun::EventReceiver eventReceiver;

		static void 	w_pressed(EntityPlayer& player) { ++player.velocity.direccion.z; }
		static void 	a_pressed(EntityPlayer& player) { --player.velocity.direccion.x; }
		static void 	s_pressed(EntityPlayer& player) { --player.velocity.direccion.z; }
		static void 	d_pressed(EntityPlayer& player) { ++player.velocity.direccion.x; }

	struct TKey2func {
		Sun::KEY_CODE key;
		void (*p_func)(EntityPlayer&);
	};

	const TKey2func keyMapping[5]
	{
		{Sun::KEY_W,                 			w_pressed },
		{Sun::KEY_A,                 			a_pressed },
		{Sun::KEY_S,                 			s_pressed },
		{Sun::KEY_D,                 			d_pressed },
		{static_cast<Sun::KEY_CODE>(0), 		nullptr	  }
	};
};