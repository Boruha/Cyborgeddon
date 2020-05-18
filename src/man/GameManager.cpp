#include <man/GameManager.hpp>
#include <sys/Systems.hpp>

#include <ratio>
#include <thread>

// TODO : input, sound y render, llevarselos a un "motor" y no tratarlos como sistemas, sino acceder a ellos a traves de eventos
// TODO : manager de eventos

void GameManager::init()
{
	engine->init(VIEWPORT_WIDTH, VIEWPORT_HEIGHT, GAME_NAME);

	currentState = &states.emplace(std::make_pair(INIT, State(INIT, entityManager))).first->second;

	currentState->registerSystem<IntroInputSystem>();
	currentState->registerSystem<HUDintroSystem>();
	currentState->registerSystem<VideoSystem>();
    currentState->registerSystem<SoundSystem>();                      // se ejecutan los sonidos en funcion de todas las cosas anteriores

	currentState->init();

	currentState = &states.emplace(std::make_pair(TUTORIAL, State(TUTORIAL, entityManager))).first->second;

	currentState->registerSystem<HUDtutorialSystem>();
	currentState->registerSystem<VideoSystem>();
	currentState->registerSystem<SoundSystem>();                      // se ejecutan los sonidos en funcion de todas las cosas anteriores

	currentState->init();

	// PAUSE

	currentState = & states.emplace(std::make_pair(PAUSE, State(PAUSE, entityManager))).first->second;

	currentState->registerSystem<PauseInputSystem>();
	currentState->registerSystem<HUDpauseSystem>();
    currentState->registerSystem<SoundSystem>();                      // se ejecutan los sonidos en funcion de todas las cosas anteriores

	currentState->init();

	// INGAME

	currentState = & states.emplace(std::make_pair(INGAME, State(INGAME, entityManager))).first->second;

	currentState->registerSystem<InputSystem>();                      // se detecta input del player
	currentState->registerSystem<AttackSystem>();                     // se ejecutan las acciones en funcion del input tanto de player como enemigos
	currentState->registerSystem<CollisionSystem>();
	currentState->registerSystem<TriggerFastCollisionSystem>();       // se ejecutan las comprobaciones de colision entre balas y resto de cosas relevantes
	currentState->registerSystem<BulletDataSystem>();                 // se actualizan los datos de las balas comprobadas anteriormente
	currentState->registerSystem<RigidBodySystem>();                  // se comprueba colision de los triggers con rigidBodies
	currentState->registerSystem<TriggerCollisionSystem>();           // se comprueba colision de los triggers entre ellos
	currentState->registerSystem<TriggerResolutionSystem>();          // se resuelve las colisiones de los triggers
	currentState->registerSystem<MovementSystem>();                   // se ejecuta el movimiento (unificar este sistema y el de abajo uniendo velocidad y fisicas)
	currentState->registerSystem<VelocitySystem>();                   // se actualiza la velocidad en funcion de la aceleracion (unificar este sistema con el de arriba pasando atributos de velocidad al cmp de fisicas)
	currentState->registerSystem<CooldownSystem>();                   // se actualizan los cooldowns
	currentState->registerSystem<AI_System>();                        // se detecta input de los enemigos
	currentState->registerSystem<DeathSystem>();                      // se comprueba si algo tiene que morir y ser eliminado
	currentState->registerSystem<SoundSystem>();                      // se ejecutan los sonidos en funcion de todas las cosas anteriores
	currentState->registerSystem<RenderSystem>();
	currentState->registerSystem<HUDingameSystem>();

//	textureManager.loadTextures();

	currentState->init();

	currentState = & states.find(INIT)->second;
}

// TODO: bucle del juego
void GameManager::run()
{
	constexpr std::chrono::duration<double, std::milli> MAX_LIMIT ( FIXED_DELTA_TIME * 1000 );
	std::chrono::duration<double, std::milli> looptime = std::chrono::milliseconds(0);

	while(engine->run()) {
		const StateEnum nextState = currentState->update();

		Timer timer { };

		if (nextState != currentState->state)
			currentState = & states.find(nextState)->second;

		looptime = std::chrono::duration<double, std::milli>(timer.getElapsedAndReset() * 10000);

		if(nextState == INGAME && looptime < MAX_LIMIT)
			std::this_thread::sleep_for(MAX_LIMIT - looptime);
		
	}
}

void GameManager::terminate() const {
//	textureManager.unloadTextures();
    engine->shutdown();
}
