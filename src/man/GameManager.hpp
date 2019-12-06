#pragma once

#include <sys/Systems.hpp>

#include <man/EntityManager.hpp>

struct GameManager {
	GameManager() = default;

	int init();
	void loop();

	RenderSystem render{};
	InputSystem input{};
	MovementSystem movement{};

	EntityManager entityManager {render.device};
};