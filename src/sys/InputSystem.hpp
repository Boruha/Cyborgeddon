#pragma once

#include <SunlightEngine/EventReceiver.hpp>

#include <Engine/util/KeyCodes.hpp>

#include <Engine/EngineInterface/IEngine.hpp>

#include <sys/System.hpp>

using glm::vec2;

struct InputSystem : System
{
	explicit InputSystem(const IEngine * const engine) : engine(engine) { }

	void init() override;
	void update(const std::unique_ptr<GameContext> &context, float deltaTime) const override;
	void reset() override {  }

	private:
		const IEngine * const engine { nullptr };

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
		KEY_CODE key;
		void (*p_func)(Entity& player, float deltaTime);
	};

	const TKey2func keyMapping[KEY_CODE_COUNT] // KEY::KODE::COUNT tiene la cantidad de teclas que usamos
	{
		{KEY_W,                    w_pressed 		},
		{KEY_A,                    a_pressed 		},
		{KEY_S,                    s_pressed 		},
		{KEY_D,                    d_pressed 		},
        {KEY_LSHIFT,               shift_pressed 	},
		{KEY_SPACE,                space_pressed 	},
		{KEY_LEFT,                 left_pressed    },
		{KEY_RIGHT,                right_pressed   },
		{KEY_M,                    m_pressed       },
		{static_cast<KEY_CODE>(0), nullptr }
	};

    void aim_mouse(Physics& phy, const vec2 &mouse) const;

    [[nodiscard]] vec3 cursorCoordToWorldCoord(float x, float y, float far) const;
};