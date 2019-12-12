#pragma once

#include <SunlightEngine/EventReceiver.hpp>
#include <SunlightEngine/KeyCodes.hpp>

#include <ent/Entities.hpp>

#include <src/man/EntityManager.hpp>

struct InputSystem
{
	explicit InputSystem(const Device& device) : device(device) {  }

	void init();
	void update(std::unique_ptr<EntityPlayer>& player);

	private:
		const Device& device;
		Sun::EventReceiver eventReceiver;

		static void 		w_pressed(std::unique_ptr<EntityPlayer>& player) { ++player->velocity.direction.z; }
		static void 		a_pressed(std::unique_ptr<EntityPlayer>& player) { --player->velocity.direction.x; }
		static void 		s_pressed(std::unique_ptr<EntityPlayer>& player) { --player->velocity.direction.z; }
		static void 		d_pressed(std::unique_ptr<EntityPlayer>& player) { ++player->velocity.direction.x; }

        static void 	    f_pressed(std::unique_ptr<EntityPlayer>& player) { if(player->velocity.speed <= 1) player->velocity.speed = 16; }

		static void 	space_pressed(std::unique_ptr<EntityPlayer>& player) { player->shooting = true; }

	struct TKey2func {
		Sun::KEY_CODE key;
		void (*p_func)(std::unique_ptr<EntityPlayer>& player);
	};

	const TKey2func keyMapping[Sun::KEY_CODE_COUNT] // KEY::KODE::COUNT tiene la cantidad de teclas que usamos
	{
		{Sun::KEY_W,                 			w_pressed 	},
		{Sun::KEY_A,                 			a_pressed 	},
		{Sun::KEY_S,                 			s_pressed 	},
		{Sun::KEY_D,                 			d_pressed 	},
        {Sun::KEY_F,                 	        f_pressed 	},
		{Sun::KEY_SPACE,                 	space_pressed 	},
		{static_cast<Sun::KEY_CODE>(0),  nullptr  	}
	};
};