#include <man/EntityManager.hpp>

void EntityManager::init()
{
    createPlayer();

    createEnemy(Vector3f(0, 0, 50));

    createCamera(Vector3f(0,20,-50));
}

void EntityManager::cleanVectors() {
	entities.erase(entities.begin(), entities.end());
	players.erase(players.begin(), players.end());
	cameras.erase(cameras.begin(), cameras.end());
}

void EntityManager::createPlayer(const Vector3f& pos, const Vector3f& dim, const float& speed) {
    players.emplace_back(std::make_unique<EntityPlayer>(device, pos, dim, speed));
}

void EntityManager::createEnemy(const Vector3f& pos, const Vector3f& dim, const float& speed) {
	enemies.emplace_back(std::make_unique<EntityEnemy>(device, pos, dim, speed));
}

void EntityManager::createCamera(const Vector3f& pos, const Vector3f& target) {
    cameras.emplace_back(std::make_unique<EntityCamera>(device, pos, target));
}

