#include <man/EntityManager.hpp>
#include <algorithm>

int EntityManager::init()
{
	players.push_back(std::make_unique<EntityPlayer>(EntityPlayer(device)));
	players.push_back(std::make_unique<EntityPlayer>(EntityPlayer(device, Vector3f(20, 0, 0), Vector3f(10), 2.f)));
	players.push_back(std::make_unique<EntityPlayer>(EntityPlayer(device, Vector3f(-20, 0, 0), Vector3f(10), 2.f)));
	players.push_back(std::make_unique<EntityPlayer>(EntityPlayer(device, Vector3f(40, 0, 0), Vector3f(10), 4.f)));
	players.push_back(std::make_unique<EntityPlayer>(EntityPlayer(device, Vector3f(-40, 0, 0), Vector3f(10), 4.f)));

	cameras.push_back(std::make_unique<EntityCamera>(EntityCamera(device)));

	return 0;
}

void EntityManager::killPlayers() {
	players.erase(std::remove_if(players.begin(),players.end(),[&](const std::unique_ptr<EntityPlayer>& pi) {
		if(pi->transformable.position.x > 50)
		{
			pi->node.removeFromScene();
			return true;
		} return false; }),players.end());
}

void EntityManager::cleanVectors() {
	entities.erase(entities.begin(), entities.end());
	players.erase(players.begin(), players.end());
	cameras.erase(cameras.begin(), cameras.end());
}