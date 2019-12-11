#include <man/EntityManager.hpp>

void EntityManager::init()
{
    createPlayer();

    createEnemy(Vector3f(0, 0, 50));

    createCamera(Vector3f(0,50,-50));
}

void EntityManager::cleanVectors() {
	entities.erase(entities.begin(), entities.end());
}

void EntityManager::createPlayer(const Vector3f& pos, const Vector3f& dim, const float& speed) {
    player.transformable.position = pos;
	player.node.setPosition(player.transformable.position);
}

void EntityManager::createEnemy(const Vector3f& pos, const Vector3f& dim, const float& speed) {
	enemies.emplace_back(std::make_unique<EntityEnemy>(device, pos, dim, speed));
}

void EntityManager::createCamera(const Vector3f& pos, const Vector3f& target) {
    camera.transformable.position = pos;
    camera.cameraNode.setPosition(camera.transformable.position);
    camera.camera.target = target;
    camera.cameraNode.setTarget(camera.camera.target);
}

