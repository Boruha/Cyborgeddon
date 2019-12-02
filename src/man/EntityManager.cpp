#include <man/EntityManager.hpp>

int EntityManager::init()
{
	camera.camera.cameraNode->setTarget(player.node->getPosition());
	entities.push_back(&player);
	return 0;
}