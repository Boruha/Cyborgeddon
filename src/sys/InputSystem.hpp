#pragma once

#include <Engine/util/KeyCodes.hpp>

#include <sys/System.hpp>

struct IEngine;

struct InputSystem : System
{
	~InputSystem() override = default;

	void init() override;
	void update(const Context &context, float deltaTime) override;
	void reset() override {  }

	private:
		// Movement
		constexpr void w_pressed(Velocity& velocity, CharacterData& data) const;
		constexpr void a_pressed(Velocity& velocity, CharacterData& data) const;
		constexpr void s_pressed(Velocity& velocity, CharacterData& data) const;
		constexpr void d_pressed(Velocity& velocity, CharacterData& data) const;
		// Dash
		constexpr void shift_pressed(Velocity& velocity, CharacterData& data) const;
		// Shoot
		constexpr void space_pressed(Velocity& velocity, CharacterData& data) const;
		// Switch Mode
		constexpr void q_pressed(Velocity& velocity, CharacterData& data) const;

		inline void aim_mouse(const Context& context, Physics& phy, const glm::vec2 &mouse) const;

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
            {KEY_Q,                    nullptr },
            {KEY_LSHIFTIRR,            nullptr },
			{KEY_LSHIFTGL,             nullptr },
            {static_cast<KEY_CODE>(0), nullptr }
        };
};