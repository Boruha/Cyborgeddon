#pragma once

#include <SunlightEngine/EventReceiver.hpp>
#include <SunlightEngine/KeyCodes.hpp>

#include <ent/Entities.hpp>

#include <sys/System.hpp>

#include <src/man/EntityManager.hpp>

struct InputSystem : System
{
	explicit InputSystem(const Device& device) : device(device) {  }

	void init() override { device.setEventReceiver(&eventReceiver); };
	void update(const std::unique_ptr<GameContext>& context) const;

	private:
		const Device& device;
		Sun::EventReceiver eventReceiver;
		//Movement
		static void 		w_pressed(std::unique_ptr<EntityPlayer>& player) 	 { ++player->velocity.direction.z; }
		static void 		a_pressed(std::unique_ptr<EntityPlayer>& player) 	 { --player->velocity.direction.x; }
		static void 		s_pressed(std::unique_ptr<EntityPlayer>& player) 	 { --player->velocity.direction.z; }
		static void 		d_pressed(std::unique_ptr<EntityPlayer>& player) 	 { ++player->velocity.direction.x; }
		//Dash
        static void 	    f_pressed(std::unique_ptr<EntityPlayer>& player) 	 { if(player->velocity.speed <= 1) player->velocity.speed = 16; }
		//Shoot
		static void 		space_pressed(std::unique_ptr<EntityPlayer>& player) { player->shooting = true; }
		//Aim
		static void 		up_pressed(std::unique_ptr<EntityPlayer>& player) 	 { ++player->transformable.rotation.z; }
		static void 		down_pressed(std::unique_ptr<EntityPlayer>& player)  { --player->transformable.rotation.z; }
		static void 		left_pressed(std::unique_ptr<EntityPlayer>& player)  { --player->transformable.rotation.x; }
		static void 		right_pressed(std::unique_ptr<EntityPlayer>& player) { ++player->transformable.rotation.x; }
		//Switch Mode
		static void 		m_pressed(std::unique_ptr<EntityPlayer>& player) 	 { player->mode = !player->mode; player->mode ? player->renderable.texture = "./img/textures/testing/testing_angel.jpg" : player->renderable.texture = "./img/textures/testing/testing_demon.jpg"; player->node.setTexture(player->renderable.texture); }

	struct TKey2func {
		Sun::KEY_CODE key;
		void (*p_func)(std::unique_ptr<EntityPlayer>& player);
	};

	const TKey2func keyMapping[Sun::KEY_CODE_COUNT] // KEY::KODE::COUNT tiene la cantidad de teclas que usamos
	{
		{Sun::KEY_W,                 			w_pressed },
		{Sun::KEY_A,                 			a_pressed },
		{Sun::KEY_S,                 			s_pressed },
		{Sun::KEY_D,                 			d_pressed },
        {Sun::KEY_F,                 	        f_pressed },
		{Sun::KEY_SPACE,                 	space_pressed },
		{Sun::KEY_UP,                 		   up_pressed },
		{Sun::KEY_DOWN,                 	 down_pressed },
		{Sun::KEY_LEFT,                 	 left_pressed },
		{Sun::KEY_RIGHT,                 	right_pressed },
		{Sun::KEY_M,                 			m_pressed },
		{static_cast<Sun::KEY_CODE>(0),  nullptr }
	};
};