#pragma once

#include <man/EntityManager.hpp>
#include <man/TextureManager.hpp>
#include <man/EngineManager.hpp>
#include <sys/RenderSystem.hpp>
#include <Engine/EngineInterface/IEngine.hpp>
#include <util/State.hpp>

struct System;

struct GameManager {

	void init();
	void run();
	void terminate() const;


	EngineManager engineManager { SUNLIGHT };
	std::unique_ptr<IEngine> engine { engineManager.getEngine() };
	Context entityManager = std::make_unique<EntityManager>(engine.get());
//    TextureManager textureManager { engine.get() };

    State * currentState { nullptr };
	std::unordered_map <StateEnum, State> states {};
};

// TODO: considerar estados de pausa, menu... donde algunos sistemas se ejecuten y otros no (punteros a funcion ???)