#include <man/GameManager.hpp>
#include <sys/Systems.hpp>
#include <Engine/EngineInterface/SceneInterface/IScene.hpp>
#include <Engine/EngineInterface/SceneInterface/IObjectNode.hpp>
#include <Engine/EngineInterface/SceneInterface/ICameraNode.hpp>
#include <Engine/util/Timer.hpp>

// TODO : input, sound y render, llevarselos a un "motor" y no tratarlos como sistemas, sino acceder a ellos a traves de eventos
// TODO : manager de eventos

void GameManager::init()
{
	engine->init(VIEWPORT_WIDTH, VIEWPORT_HEIGHT, GAME_NAME);

	currentState = & states.emplace(INGAME, INGAME).first->second;

	currentState->registerSystem(std::make_unique<InputSystem>(engine.get())); // se detecta input del player
	currentState->registerSystem(std::make_unique<AI_System>());                        // se detecta input de los enemigos
	currentState->registerSystem(std::make_unique<AttackSystem>());                     // se ejecutan las acciones en funcion del input tanto de player como enemigos
	currentState->registerSystem(std::make_unique<TriggerFastCollisionSystem>());       // se ejecutan las comprobaciones de colision entre balas y resto de cosas relevantes
	currentState->registerSystem(std::make_unique<BulletDataSystem>());                 // se actualizan los datos de las balas comprobadas anteriormente
	currentState->registerSystem(std::make_unique<RigidBodySystem>());                  // se comprueba colision de los triggers con rigidBodies
	currentState->registerSystem(std::make_unique<TriggerCollisionSystem>());           // se comprueba colision de los triggers entre ellos
	currentState->registerSystem(std::make_unique<TriggerResolutionSystem>());          // se resuelve las colisiones de los triggers
	currentState->registerSystem(std::make_unique<MovementSystem>());                   // se ejecuta el movimiento (unificar este sistema y el de abajo uniendo velocidad y fisicas)
	currentState->registerSystem(std::make_unique<VelocitySystem>());                   // se actualiza la velocidad en funcion de la aceleracion (unificar este sistema con el de arriba pasando atributos de velocidad al cmp de fisicas)
	currentState->registerSystem(std::make_unique<CooldownSystem>());                   // se actualizan los cooldowns
	currentState->registerSystem(std::make_unique<DeathSystem>());                      // se comprueba si algo tiene que morir y ser eliminado
	currentState->registerSystem(std::make_unique<SoundSystem>());                      // se ejecutan los sonidos en funcion de todas las cosas anteriores
	currentState->registerSystem(std::make_unique<RenderSystem>(engine.get()));

	currentState->setContext(entityManager);

	textureManager.loadTextures();

	currentState->init();
/*
	systems.reserve(13); // TODO cambiar este valor al anadir un nuevo sistema
	                                                                            // ORDEN DE EJECUCION
	systems.emplace_back(std::make_unique<InputSystem>(engine.get()));          // se detecta input del player
	systems.emplace_back(std::make_unique<AI_System>());                        // se detecta input de los enemigos
	systems.emplace_back(std::make_unique<AttackSystem>());                     // se ejecutan las acciones en funcion del input tanto de player como enemigos
	systems.emplace_back(std::make_unique<TriggerFastCollisionSystem>());       // se ejecutan las comprobaciones de colision entre balas y resto de cosas relevantes
	systems.emplace_back(std::make_unique<BulletDataSystem>());                 // se actualizan los datos de las balas comprobadas anteriormente
	systems.emplace_back(std::make_unique<RigidBodySystem>());                  // se comprueba colision de los triggers con rigidBodies
    systems.emplace_back(std::make_unique<TriggerCollisionSystem>());           // se comprueba colision de los triggers entre ellos
    systems.emplace_back(std::make_unique<TriggerResolutionSystem>());          // se resuelve las colisiones de los triggers
	systems.emplace_back(std::make_unique<MovementSystem>());                   // se ejecuta el movimiento (unificar este sistema y el de abajo uniendo velocidad y fisicas)
	systems.emplace_back(std::make_unique<VelocitySystem>());                   // se actualiza la velocidad en funcion de la aceleracion (unificar este sistema con el de arriba pasando atributos de velocidad al cmp de fisicas)
	systems.emplace_back(std::make_unique<CooldownSystem>());                   // se actualizan los cooldowns
    systems.emplace_back(std::make_unique<DeathSystem>());                      // se comprueba si algo tiene que morir y ser eliminado
	systems.emplace_back(std::make_unique<SoundSystem>());                      // se ejecutan los sonidos en funcion de todas las cosas anteriores
	systems.emplace_back(std::make_unique<RenderSystem>(glm::vec2(VIEWPORT_WIDTH, VIEWPORT_HEIGHT), GAME_NAME, engine.get(), 0));

	for(const auto& sys : systems)
		sys->init();


	entityManager->init();*/
}

// TODO: bucle del juego
void GameManager::run()
{

	while(engine->run()) {
		const StateEnum nextState = currentState->update();

		if (nextState != currentState->state)
			currentState = & states.find(currentState->update())->second;
	}
}

void GameManager::terminate() {
	textureManager.unloadTextures();
    engine->shutdown();
}
