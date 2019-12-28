#pragma once

#include <sys/Systems.hpp>

#include <util/GameContext.hpp>
#include <man/EntityManager.hpp>

#include <chrono>

struct GameManager {
	GameManager() = default;
	~GameManager() = default;

	void init();
	void update();
	void loop();

	// TODO: bucle del juego
	//static constexpr unsigned int UPS = 30;
	//static constexpr std::chrono::milliseconds TICK_MS = std::chrono::milliseconds(1000 / UPS);

	// render first so we use its device for the other systems and/or managers
	RenderSystem render { Sun::Vector2u(1280, 720), L"Cyborgeddon" };
	SynchronizeNodeSystem sync {  };

	std::vector<std::unique_ptr<System>> systems;

	std::unique_ptr<GameContext> entityManager = std::make_unique<EntityManager>(render.device);
};

// TODO: considerar estados de pausa, menu... donde algunos sistemas se ejecuten y otros no (punteros a funcion)