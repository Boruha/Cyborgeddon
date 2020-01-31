#pragma once

#include <man/EntityManager.hpp>
#include <sys/RenderSystem.hpp>
#include <chrono>

struct System;

struct GameManager {
	GameManager() = default;
	~GameManager() = default;

	void init();
	void update(float deltaTime);
	void loop();

	static constexpr unsigned int UPS = 60;
	static constexpr std::chrono::duration<float> TICK_MS { 1.f / UPS };

	// render first so we use its device for the other systems and/or managers
	RenderSystem render { Sun::Vector2u(1280, 720), L"Cyborgeddon" };

	std::vector<std::unique_ptr<System>> systems;

	std::unique_ptr<GameContext> entityManager = std::make_unique<EntityManager>(render.device);
};

// TODO: considerar estados de pausa, menu... donde algunos sistemas se ejecuten y otros no (punteros a funcion)