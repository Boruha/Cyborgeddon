#pragma once

#include <Engine/util/KeyCodes.hpp>

#include <sys/System.hpp>

struct IEngine;

using glm::vec2;

struct InputSystem : System
{
	explicit InputSystem(const IEngine * const engine) : engine(engine) { }

	void init() override;
	void update(const std::unique_ptr<GameContext> &context, float deltaTime) override;
	void reset() override {  }

	private:
		const IEngine * const engine { nullptr };

		// Movement
		void w_pressed(Entity& player, float deltaTime) const;
		void a_pressed(Entity& player, float deltaTime) const;
		void s_pressed(Entity& player, float deltaTime) const;
		void d_pressed(Entity& player, float deltaTime) const;
		// Dash
		void shift_pressed(Entity& player, float deltaTime) const;
		// Shoot
		void space_pressed(Entity& player, float deltaTime) const;
		// Switch Mode
		void m_pressed(Entity& player, float deltaTime) const;

	struct TKey2func {
		KEY_CODE key;
		void (InputSystem::*p_func)(Entity& player, float deltaTime) const;
	};

	TKey2func keyMap[KEY_CODE_COUNT] {
        {KEY_W,                    nullptr },
        {KEY_A,                    nullptr },
        {KEY_S,                    nullptr },
        {KEY_D,                    nullptr },
        {KEY_LSHIFT,               nullptr },
        {KEY_SPACE,                nullptr },
        {KEY_M,                    nullptr },
        {static_cast<KEY_CODE>(0), nullptr }
    };

    void aim_mouse(Physics& phy, const vec2 &mouse) const;
};