#pragma once

#include <Engine/util/KeyCodes.hpp>

#include <sys/System.hpp>

struct IEngine;

struct InputSystem : System
{
	explicit InputSystem(IEngine * const engine) : engine(*engine) { }

	~InputSystem() override = default;

	void init() override;
	void update(const Context &context, float deltaTime) override;
	void reset() override {  }

	private:
		IEngine& engine; // no puede ser const porque tiene que acceder al input (el cual no puede ser const de momento)

		// Movement
		void w_pressed(Velocity& velocity, CharacterData& data) const;
		void a_pressed(Velocity& velocity, CharacterData& data) const;
		void s_pressed(Velocity& velocity, CharacterData& data) const;
		void d_pressed(Velocity& velocity, CharacterData& data) const;
		// Dash
		void shift_pressed(Velocity& velocity, CharacterData& data) const;
		// Shoot
		void space_pressed(Velocity& velocity, CharacterData& data) const;
		// Switch Mode
		void m_pressed(Velocity& velocity, CharacterData& data) const;

        void aim_mouse(Physics& phy, const glm::vec2 &mouse) const;

        struct TKey2func {
            KEY_CODE key;
            void (InputSystem::*p_func)(Velocity& velocity, CharacterData& data) const;
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