#pragma once

#include <sys/Systems.hpp>

#include <man/EntityManager.hpp>

#include <chrono>

struct GameManager {
	GameManager() = default;
	~GameManager() = default;

	void init();
	void update();
	void loop();

	static constexpr unsigned int UPS = 30;
	static constexpr std::chrono::milliseconds TICK_MS = std::chrono::milliseconds(1000 / UPS);

	// render first so we use its device for the other systems and/or managers
	RenderSystem render { Sun::Vector2u(640, 480), L"Cyborgeddon" };
	InputSystem input { render.device };
	MovementSystem movement {  };

	EntityManager entityManager { render.device };
};