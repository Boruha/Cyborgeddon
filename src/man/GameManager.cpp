#include <man/GameManager.hpp>

void GameManager::init()
{
	input.init();
	render.init();

	entityManager.init();
}

void GameManager::update()
{
	entityManager.killPlayers();
	input.update(entityManager.getPlayers());
	collision.update(entityManager.getPlayers(), entityManager.getDoors(), entityManager.getKeys());
	movement.update(entityManager.getPlayers());
}

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
	}
}