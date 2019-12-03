#pragma once

#include <sys/Systems.hpp>

#include <man/EntityManager.hpp>

struct GameManager {
	GameManager() = default;

	int init();
	void update();
	// void status();
	// void pause();

	RenderSystem render{};
	InputSystem input{};
	MovementSystem movement{};

	EntityManager entityManager = EntityManager(render.device);
};