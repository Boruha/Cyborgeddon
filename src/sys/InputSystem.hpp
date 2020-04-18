#pragma once

#include <Engine/util/KeyCodes.hpp>

#include <sys/System.hpp>

struct IEngine;

struct InputSystem : System
{
	~InputSystem() override = default;

	void init() override;
	void fixedUpdate(const Context &context, float deltaTime) override;
	void update(const Context &context, float deltaTime) override {};
	void reset() override {  }

	private:
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
		void q_pressed(Velocity& velocity, CharacterData& data) const;

		inline void aim_mouse(const Context& context, Physics& phy, const glm::vec2 &mouse) const;

        struct TKey2func {
            unsigned key;
	        void (InputSystem::*p_func)(Velocity& velocity, CharacterData& data) const;
        };

        TKey2func keyMap[KEY_CODE_COUNT] {
            { KEY_W,                    nullptr },
            { KEY_A,                    nullptr },
            { KEY_S,                    nullptr },
            { KEY_D,                    nullptr },
            { KEY_SPACE,                nullptr },
            { KEY_Q,                    nullptr },
			{ KEY_LSHIFT,               nullptr },
            { 0,                        nullptr }
        };
};