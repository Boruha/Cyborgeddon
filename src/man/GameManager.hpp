#pragma once

#include <sys/Systems.hpp>

#include <man/EntityManager.hpp>

struct GameManager {
	GameManager() = default;
	~GameManager() = default;

	int init();
	void loop();

	RenderSystem render { Sun::Vector2u(640, 480), L"Cyborgeddon" };
	InputSystem input {  };
	MovementSystem movement {  };

	EntityManager entityManager { render.device };
};