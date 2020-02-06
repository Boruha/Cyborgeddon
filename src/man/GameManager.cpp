#include <man/GameManager.hpp>
#include <sys/Systems.hpp>
//#include <IEngine/EngineInterface/IEngine.hpp>
//#include <IEngine/EngineInterface/SceneInterface/IScene.hpp>
//#include <IEngine/EngineInterface/SceneInterface/IObjectNode.hpp>
//#include <IEngine/EngineInterface/SceneInterface/ICameraNode.hpp>

// TODO : input, sound y render, llevarselos a un "motor" y no tratarlos como sistemas, sino acceder a ellos a traves de eventos
// TODO : manager de eventos

void GameManager::init()
{
    textureManager.loadTextures();

	systems.reserve(10); // TODO cambiar este valor al anadir un nuevo sistema
	                                                                            // ORDEN DE EJECUCION
	systems.emplace_back(std::make_unique<InputSystem>(render.device));      // se detecta input del player
	systems.emplace_back(std::make_unique<AI_System>());                        // se detecta input de los enemigos
	systems.emplace_back(std::make_unique<ActionSystem>());                     // se ejecutan las acciones en funcion del input tanto de player como enemigos
	systems.emplace_back(std::make_unique<HighSpeedCollisionSystem>());         // se controla la colision de las balas con enemigos (de momento)
	systems.emplace_back(std::make_unique<CollisionSystem>());                  // se controla colision de cosas - paredes, cosas - cosas (dividir en 2 en el futuro)
	systems.emplace_back(std::make_unique<MovementSystem>());                   // se ejecuta el movimiento (unificar este sistema y el de abajo uniendo velocidad y fisicas)
	systems.emplace_back(std::make_unique<VelocitySystem>());                   // se actualiza la velocidad en funcion de la aceleracion (unificar este sistema con el de arriba pasando atributos de velocidad al cmp de fisicas)
	systems.emplace_back(std::make_unique<CooldownSystem>());                   // se actualizan los cooldowns
    systems.emplace_back(std::make_unique<DeathSystem>());                      // se comprueba si algo tiene que morir y ser eliminado
	systems.emplace_back(std::make_unique<SoundSystem>());                      // se ejecutan los sonidos en funcion de todas las cosas anteriores

	for(auto& sys : systems)
		sys->init();


	entityManager->init();

	render.init();

//    engine->init(VIEWPORT_WIDTH, VIEWPORT_HEIGHT, GAME_NAME);
//    textureManager.loadTextures(engine.get());
}

void GameManager::update(const float deltaTime)
{
    // Ejecucion de los sistemas
	for(auto& sys : systems)
		sys->update(entityManager, deltaTime);

	// reseteo de nivel al ganar (esto es momentaneo)
	// entity manager crea y destruye entidades en funcion de lo que hagan los sistemas
	if(entityManager->update()) {			// TODO : hacer maquina de estados para permitir estado inicio, pausa, en juego...
		for (auto& sys : systems)
			sys->reset();
		entityManager->createLevel();
	}
}

// TODO: bucle del juego
void GameManager::loop()
{
	std::chrono::high_resolution_clock::time_point last = std::chrono::high_resolution_clock::now();
	std::chrono::high_resolution_clock ::time_point now;

	std::chrono::duration<float> delta {0};
/*
	glm::vec3 pla(0);
	glm::vec3 rot(0);
	glm::vec3 sca(1);

	glm::vec3 cam(5);

	std::unique_ptr<INode> player = engine->scene->addObjectNode(&pla, &rot, &sca);
	std::unique_ptr<INode> camera = engine->scene->addCameraNode(&cam, &rot, &sca, &pla);

	while (engine->run()) {
        now = std::chrono::high_resolution_clock::now();
        delta += now - last;
        last = now;

        while(delta.count() > fixedDelta.count())
        {
            update(fixedDelta.count());
            delta -= fixedDelta;
        }

         engine->clear(Color(WHITE));
         engine->draw();
         engine->display();
	}

	terminate();
*/

	while(render.device.isActive())
	{
		now = std::chrono::high_resolution_clock::now();
		delta += now - last;
		last = now;

		while(delta.count() > fixedDelta.count())
		{
			update(fixedDelta.count());
			delta -= fixedDelta;
		}

		render.updateFPS(delta.count());
		render.update(entityManager, delta.count() / fixedDelta.count());
  	}
}

void GameManager::terminate() {
//    engine->shutdown();
}
