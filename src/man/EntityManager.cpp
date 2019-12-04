#include <man/EntityManager.hpp>

int EntityManager::init()
{
	camera.camera.cameraNode->setTarget(player.node->getPosition());
	cube.node->setPosition(irr::core::vector3df(20,0,20));
	return 0;
}