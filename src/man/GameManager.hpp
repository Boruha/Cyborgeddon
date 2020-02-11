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
	~GameManager() { terminate(); };

	void init();
	void update(float deltaTime);
	void loop();
	void terminate();

	const std::chrono::duration<float> fixedDelta { FIXED_DELTA_TIME };

	// render first so we use its device for the other systems and/or managers

	std::vector<std::unique_ptr<System>> systems;

	EngineManager engineManager { IRRLICHT };
	std::unique_ptr<IEngine> engine = engineManager.getEngine();
    TextureManager textureManager { engine.get() };

	RenderSystem render { glm::vec2(VIEWPORT_WIDTH, VIEWPORT_HEIGHT), GAME_NAME, engine.get() };
	std::unique_ptr<GameContext> entityManager = std::make_unique<EntityManager>(engine.get());
};

// TODO: considerar estados de pausa, menu... donde algunos sistemas se ejecuten y otros no (punteros a funcion)