#pragma once

#include <man/EntityManager.hpp>
#include <man/TextureManager.hpp>
#include <man/EngineManager.hpp>
#include <sys/RenderSystem.hpp>
#include <Engine/EngineInterface/IEngine.hpp>
#include <chrono>

struct System;

struct GameManager {
	GameManager() = default;
	~GameManager() = default;

	void init();
	void update(float deltaTime);
	void loop();
	void terminate();

	const std::chrono::duration<float> fixedDelta { FIXED_DELTA_TIME };

	// render first so we use its device for the other systems and/or managers
	RenderSystem render { Vector2u(VIEWPORT_WIDTH, VIEWPORT_HEIGHT), GAME_NAME };

	std::vector<std::unique_ptr<System>> systems;

	std::unique_ptr<GameContext> entityManager = std::make_unique<EntityManager>(render.device);

	EngineManager engineManager { IRRLICHT };
	std::unique_ptr<IEngine> engine = engineManager.getEngine();
    TextureManager textureManager { engine.get() };
};

// TODO: considerar estados de pausa, menu... donde algunos sistemas se ejecuten y otros no (punteros a funcion)