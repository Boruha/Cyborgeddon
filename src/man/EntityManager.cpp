#include <man/EntityManager.hpp>
#include <algorithm>

int EntityManager::init()
{
	players.push_back(std::make_unique<EntityPlayer>(EntityPlayer(device)));
	players.push_back(std::make_unique<EntityPlayer>(EntityPlayer(device, Vector3f(20, 0, 0), Vector3f(10), 2.f)));
	players.push_back(std::make_unique<EntityPlayer>(EntityPlayer(device, Vector3f(-20, 0, 0), Vector3f(10), 2.f)));
	players.push_back(std::make_unique<EntityPlayer>(EntityPlayer(device, Vector3f(40, 0, 0), Vector3f(10), 4.f)));
	players.push_back(std::make_unique<EntityPlayer>(EntityPlayer(device, Vector3f(-40, 0, 0), Vector3f(10), 4.f)));
	return 0;
}

void EntityManager::killPlayers() {
	players.erase(std::remove_if(players.begin(),players.end(),[&](std::unique_ptr<EntityPlayer>const& pi) {
		if(pi->transformable.position.x > 50)
		{
			device->getSceneManager()->addToDeletionQueue(pi->node);
			return true;
		} return false; }),players.end());
}
