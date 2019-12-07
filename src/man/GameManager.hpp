#pragma once

#include <sys/Systems.hpp>

#include <man/EntityManager.hpp>

struct GameManager {
	GameManager() = default;

	int init();
	void update();

	RenderSystem render{};
	InputSystem input{};
	MovementSystem movement{};
	AI_System ai{};

	EntityManager entityManager = EntityManager(render.device);
};