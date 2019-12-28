#include <man/GameManager.hpp>

void GameManager::init()
{
	systems.emplace_back(std::make_unique<InputSystem>(InputSystem(render.device)));
	systems.emplace_back(std::make_unique<AI_System>());
	systems.emplace_back(std::make_unique<CollisionSystem>());
	systems.emplace_back(std::make_unique<MovementSystem>());
	systems.emplace_back(std::make_unique<CooldownSystem>());
//	systems.emplace_back(std::make_unique<SoundSystem>(SoundSystem()));

	for(auto& sys : systems)
		sys->init();


	entityManager->init();

	sync.init();
	render.init();
}

void GameManager::update()
{
	for(auto& sys : systems)
		sys->update(entityManager);

	// entity manager crea y destruye entidades en funcion de lo que hagan los sistemas
	entityManager->update();
	// tras modificar las entidades, hay que sincronizar nuestros datos con los nodos de la escena antes del render
	sync.update(entityManager);
}

// TODO: bucle del juego
void GameManager::loop()
{/*
	std::chrono::milliseconds lag (0);
	std::chrono::milliseconds elapsed;

	std::chrono::time_point<std::chrono::high_resolution_clock> last = std::chrono::high_resolution_clock::now();
	std::chrono::time_point<std::chrono::high_resolution_clock> now;
*/
	while(render.device.isActive())
	{/*
		now = std::chrono::high_resolution_clock::now();
		elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - last);
		last = now;
		lag += elapsed;


		while(lag > TICK_MS)
		{*/
			update();
		/*	lag -= TICK_MS;
		}*/

		render.update(entityManager);
		// TODO: interpolar movimiento en render
	}
}