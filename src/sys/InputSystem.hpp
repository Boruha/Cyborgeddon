#pragma once

#include <SunlightEngine/EventReceiver.hpp>
#include <SunlightEngine/KeyCodes.hpp>

#include <ent/EntityPlayer.hpp>

#include <sys/System.hpp>

#include <src/man/EntityManager.hpp>

#include <sys/CooldownSystem.hpp>

struct InputSystem : System
{
	explicit InputSystem(const Device& device) : device(device) {  }

	void init() override;
	void update(const std::unique_ptr<GameContext>& context) const override;

	private:
		const Device& device;
		Sun::EventReceiver eventReceiver;
		// Movement
		static void w_pressed(EntityPlayer& player);
		static void a_pressed(EntityPlayer& player);
		static void s_pressed(EntityPlayer& player);
		static void d_pressed(EntityPlayer& player);
		// Dash
        static void shift_pressed(EntityPlayer& player);
		// Shoot
		static void space_pressed(EntityPlayer& player);
		// Aim
		static void left_pressed  (EntityPlayer& player);
		static void right_pressed (EntityPlayer& player);
		// Switch Mode
		static void m_pressed(EntityPlayer& player);

	struct TKey2func {
		Sun::KEY_CODE key;
		void (*p_func)(EntityPlayer& player);
	};

	const TKey2func keyMapping[Sun::KEY_CODE_COUNT] // KEY::KODE::COUNT tiene la cantidad de teclas que usamos
	{
		{Sun::KEY_W,                 			w_pressed },
		{Sun::KEY_A,                 			a_pressed },
		{Sun::KEY_S,                 			s_pressed },
		{Sun::KEY_D,                 			d_pressed },
        {Sun::KEY_LSHIFT,                 	shift_pressed },
		{Sun::KEY_SPACE,                 	space_pressed },
		{Sun::KEY_LEFT,                 	 left_pressed },
		{Sun::KEY_RIGHT,                 	right_pressed },
		{Sun::KEY_M,                 			m_pressed },
		{static_cast<Sun::KEY_CODE>(0),  nullptr }
	};
};