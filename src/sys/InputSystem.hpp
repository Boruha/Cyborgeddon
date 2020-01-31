#pragma once

#include <SunlightEngine/EventReceiver.hpp>
#include <SunlightEngine/KeyCodes.hpp>

#include <sys/System.hpp>

namespace Sun {
    struct Device;
}

struct InputSystem : System
{
	explicit InputSystem(const Sun::Device& device) : device(device) {  }

	void init() override;
	void update(const std::unique_ptr<GameContext> &context, float deltaTime) const override;
	void reset() override {  }

	private:
		const Sun::Device& device;
		Sun::EventReceiver eventReceiver;
		// Movement
		static void w_pressed(Entity& player, float deltaTime);
		static void a_pressed(Entity& player, float deltaTime);
		static void s_pressed(Entity& player, float deltaTime);
		static void d_pressed(Entity& player, float deltaTime);
		// Dash
        static void shift_pressed(Entity& player, float deltaTime);
		// Shoot
		static void space_pressed(Entity& player, float deltaTime);
		// Aim
		static void left_pressed  (Entity& player, float deltaTime);
		static void right_pressed (Entity& player, float deltaTime);
		// Switch Mode
		static void m_pressed(Entity& player, float deltaTime);

	struct TKey2func {
		Sun::KEY_CODE key;
		void (*p_func)(Entity& player, float deltaTime);
	};

	const TKey2func keyMapping[Sun::KEY_CODE_COUNT] // KEY::KODE::COUNT tiene la cantidad de teclas que usamos
	{
		{Sun::KEY_W,                    w_pressed 		},
		{Sun::KEY_A,                    a_pressed 		},
		{Sun::KEY_S,                    s_pressed 		},
		{Sun::KEY_D,                    d_pressed 		},
        {Sun::KEY_LSHIFT,               shift_pressed 	},
		{Sun::KEY_SPACE,                space_pressed 	},
		{Sun::KEY_LEFT,                 left_pressed 	},
		{Sun::KEY_RIGHT,                right_pressed 	},
		{Sun::KEY_M,                    m_pressed 		},
		{static_cast<Sun::KEY_CODE>(0), nullptr}
	};

    void aim_mouse(Physics& phy, const Sun::Vector2u &mouse) const;

    void cursorCoordToWorldCoord(float x, float y, float far, Vector3f &worldCoordinates) const;
};