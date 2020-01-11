#include <man/GameManager.hpp>

void GameManager::init()
{
	systems.emplace_back(std::make_unique<VelocitySystem>()); // de momento lo ponemos aqui para que input y AI utilicen la velocidad
	systems.emplace_back(std::make_unique<InputSystem>(render.device));
	systems.emplace_back(std::make_unique<AI_System>());
	systems.emplace_back(std::make_unique<HighSpeedCollisionSystem>());
	systems.emplace_back(std::make_unique<CollisionSystem>());
	systems.emplace_back(std::make_unique<MovementSystem>());
	systems.emplace_back(std::make_unique<CooldownSystem>());
	systems.emplace_back(std::make_unique<BulletSystem>());
	systems.emplace_back(std::make_unique<SoundSystem>());

	for(auto& sys : systems)
		sys->init();


	entityManager->init();


	render.init();
}

void GameManager::update(const float deltaTime)
{
	for(auto& sys : systems)
		sys->update(entityManager, deltaTime);

	// entity manager crea y destruye entidades en funcion de lo que hagan los sistemas
	entityManager->update();
}

// TODO: bucle del juego
void GameManager::loop()
{
	std::chrono::high_resolution_clock::time_point last = std::chrono::high_resolution_clock::now();
	std::chrono::high_resolution_clock ::time_point now;

	std::chrono::duration<float> delta {0};

	while(render.device.isActive())
	{
		now = std::chrono::high_resolution_clock::now();
		delta += now - last;
		last = now;

		while(delta.count() > TICK_MS.count())
		{
			update(delta.count());
			delta -= TICK_MS;
		}

		render.update(entityManager, delta.count() / TICK_MS.count());
		// TODO: interpolar movimiento en render
  	}
}