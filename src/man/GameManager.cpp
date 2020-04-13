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

	// PAUSE

	currentState = & states.emplace(std::make_pair(PAUSE, State(PAUSE, entityManager))).first->second;

	currentState->init();

	// INGAME

	currentState = & states.emplace(std::make_pair(INGAME, State(INGAME, entityManager))).first->second;

	currentState->registerSystem<InputSystem>();                      // se detecta input del player
	currentState->registerSystem<AI_System>();                        // se detecta input de los enemigos
	currentState->registerSystem<SchedulingSystem>();                 // actualizacion phases de IAs (Scheduling)
	currentState->registerSystem<AttackSystem>();                     // se ejecutan las acciones en funcion del input tanto de player como enemigos
	currentState->registerSystem<TriggerFastCollisionSystem>();       // se ejecutan las comprobaciones de colision entre balas y resto de cosas relevantes
	currentState->registerSystem<BulletDataSystem>();                 // se actualizan los datos de las balas comprobadas anteriormente
	currentState->registerSystem<RigidBodySystem>();                  // se comprueba colision de los triggers con rigidBodies
	currentState->registerSystem<TriggerCollisionSystem>();           // se comprueba colision de los triggers entre ellos
	currentState->registerSystem<TriggerResolutionSystem>();          // se resuelve las colisiones de los triggers
	currentState->registerSystem<MovementSystem>();                   // se ejecuta el movimiento (unificar este sistema y el de abajo uniendo velocidad y fisicas)
	currentState->registerSystem<VelocitySystem>();                   // se actualiza la velocidad en funcion de la aceleracion (unificar este sistema con el de arriba pasando atributos de velocidad al cmp de fisicas)
	currentState->registerSystem<CooldownSystem>();                   // se actualizan los cooldowns
	currentState->registerSystem<DeathSystem>();                      // se comprueba si algo tiene que morir y ser eliminado
	currentState->registerSystem<SoundSystem>();                      // se ejecutan los sonidos en funcion de todas las cosas anteriores
	currentState->registerSystem<RenderSystem>();

//	textureManager.loadTextures();

	currentState->init();
}

// TODO: bucle del juego
void GameManager::run()
{
	while(engine->run()) {
		const StateEnum nextState = currentState->update();

		if (nextState != currentState->state)
			currentState = & states.find(nextState)->second;
	}
}

void GameManager::terminate() {
//	textureManager.unloadTextures();
    engine->shutdown();
}
