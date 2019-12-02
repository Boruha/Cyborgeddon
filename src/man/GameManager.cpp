#include <man/GameManager.hpp>
#include <iostream>

using std::cerr;
using std::endl;

int GameManager::init()
{
	if(render.init() == 1)
	{
		cerr << "Error en render init" << endl;
		return 1;
	}

	if(input.init(render.device) == 1)
	{
		cerr << "Error en input init" << endl;
		return 1;
	}

	if(entityManager.init() == 1)
	{
		cerr << "Error en entity manager init" << endl;
		return 1;
	}

	return 0;
}

void GameManager::update()
{
	while(render.device->run())
	{
		input.update(entityManager.player);
		movement.update(entityManager.player);
		render.update();
	}
}