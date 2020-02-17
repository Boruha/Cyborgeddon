#pragma once

#include <Engine/util/KeyCodes.hpp>

#include <sys/System.hpp>

struct IEngine;

using glm::vec2;

struct InputSystem : System
{
	explicit InputSystem(IEngine * const engine) : engine(engine) { }

	void init() override;
	void update(const std::unique_ptr<GameContext> &context, float deltaTime) override;
	void reset() override {  }

	private:
		IEngine * const engine {nullptr };

		// Movement
		void w_pressed(Entity& player) const;
		void a_pressed(Entity& player) const;
		void s_pressed(Entity& player) const;
		void d_pressed(Entity& player) const;
		// Dash
		void shift_pressed(Entity& player) const;
		// Shoot
		void space_pressed(Entity& player) const;
		// Switch Mode
		void m_pressed(Entity& player) const;

        void aim_mouse(Physics& phy, const vec2 &mouse) const;

        struct TKey2func {
            KEY_CODE key;
            void (InputSystem::*p_func)(Entity& player) const;
        };

        TKey2func keyMap[KEY_CODE_COUNT] {
            {KEY_W,                    nullptr },
            {KEY_A,                    nullptr },
            {KEY_S,                    nullptr },
            {KEY_D,                    nullptr },
            {KEY_SPACE,                nullptr },
            {KEY_M,                    nullptr },
            {KEY_LSHIFTIRR,            nullptr },
			{KEY_LSHIFTGL,             nullptr },
            {static_cast<KEY_CODE>(0), nullptr }
        };
};