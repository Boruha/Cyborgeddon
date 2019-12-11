#include <man/GameManager.hpp>

void GameManager::init()
{
	input.init();
	ai.init();
	render.init();

	entityManager.init();
}

void GameManager::update()
{
	input.update(entityManager.getPlayer());
	ai.update(entityManager.getPlayer(), entityManager.getEnemies());
	movement.update(entityManager.getPlayer());
	movement.update(entityManager.getEnemies());
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

		render.update();
		// TODO: interpolar movimiento en render
	}
}