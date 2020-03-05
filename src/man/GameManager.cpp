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
	textureManager.loadTextures();

	systems.reserve(13); // TODO cambiar este valor al anadir un nuevo sistema
	                                                                            // ORDEN DE EJECUCION
	systems.emplace_back(std::make_unique<InputSystem>(engine.get())); // se detecta input del player
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
//	systems.emplace_back(std::make_unique<SoundSystem>());                      // se ejecutan los sonidos en funcion de todas las cosas anteriores

	for(const auto& sys : systems)
		sys->init();


	entityManager->init();

	render.init();
}

void GameManager::update(const float deltaTime)
{
    // Ejecucion de los sistemas
	for(const auto& sys : systems)
		sys->update(entityManager, deltaTime);

	// reseteo de nivel al ganar (esto es momentaneo)
	// entity manager crea y destruye entidades en funcion de lo que hagan los sistemas
	if(entityManager->update()) {			// TODO : hacer maquina de estados para permitir estado inicio, pausa, en juego...
		for (const auto& sys : systems)
			sys->reset();
		entityManager->createLevel();
	}
}

// TODO: bucle del juego
void GameManager::loop()
{
	Timer gameClock { duration::seconds }; // reloj que controla el juego

	Timer timerUpdateLoop { duration::milliseconds }; // reloj para comprobar coste temporal

	double delta { 0 };

	while (engine->run()) {
		delta += gameClock.getElapsedAndReset();

        while(delta > FIXED_DELTA_TIME)
        {
	        timerUpdateLoop.start();
            update(FIXED_DELTA_TIME);
	        timerUpdateLoop.getInfo();

            delta -= FIXED_DELTA_TIME;
        }

        render.update(entityManager, float(delta / FIXED_DELTA_TIME));
	}

	terminate();
}

void GameManager::terminate() {
	textureManager.unloadTextures();
    engine->shutdown();
}
