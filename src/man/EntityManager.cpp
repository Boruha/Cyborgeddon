#include <man/EntityManager.hpp>
#include <algorithm>

void EntityManager::init()
{
    createPlayer();
    //createPlayer(Vector3f(10,0,0), Vector3f(5), 2.f);
	//createPlayer(Vector3f(-10, 0, 0), Vector3f(5), 2.f);
    //createPlayer(Vector3f(20, 0, 0), Vector3f(5), 4.f);
    //createPlayer(Vector3f(-20, 0, 0), Vector3f(5), 4.f);

    createCamera(Vector3f(0,20,-50));
}

void EntityManager::killPlayers() {
	players.erase(std::remove_if(players.begin(),players.end(),[&](const std::unique_ptr<EntityPlayer>& pi) {return pi->transformable.position.x > 50;}),players.end());
}

void EntityManager::cleanVectors() {
	entities.erase(entities.begin(), entities.end());
	players.erase(players.begin(), players.end());
	cameras.erase(cameras.begin(), cameras.end());
}

void EntityManager::createPlayer(const Vector3f& pos, const Vector3f& dim, const float& speed) {
    players.emplace_back(std::make_unique<EntityPlayer>(device, pos, dim, speed));
}

void EntityManager::createCamera(const Vector3f& pos, const Vector3f& target) {
    cameras.emplace_back(std::make_unique<EntityCamera>(device, pos, target));
}
