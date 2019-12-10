#include <man/EntityManager.hpp>

int EntityManager::init()
{
	camera.camera.cameraNode->setTarget(player.node->getPosition());
	return 0;
}