#include <man/EntityManager.hpp>
#include <algorithm>

void EntityManager::init()
{
    createPlayer();
    createDoor(Vector3f(20,0,10));
    createDoor(Vector3f(-20,0,5));
    createDoor(Vector3f(-20,0,15));
    //createPlayer(Vector3f(10,0,0), Vector3f(5), 2.f);
    createKey(Vector3f(0,0,10));
	//createPlayer(Vector3f(-10, 0, 0), Vector3f(5), 2.f);
    //createPlayer(Vector3f(20, 0, 0), Vector3f(5), 4.f);
    //createPlayer(Vector3f(-20, 0, 0), Vector3f(5), 4.f);

    createCamera(Vector3f(0,20,-50));
}

void EntityManager::killPlayers() {
	players.erase(std::remove_if(players.begin(),players.end(),[&](const std::unique_ptr<EntityPlayer>& pi) {return pi->transformable.position.x > 50;}),players.end());
}

void EntityManager::takeKey(){
    for(int i =0; i < keys.size(); i++){
        if(keys.at(i)->taken){
            keys.erase(keys.begin() + i);
        }
    }
}

void EntityManager::cleanVectors() {
	entities.erase(entities.begin(), entities.end());
	players.erase(players.begin(), players.end());
	cameras.erase(cameras.begin(), cameras.end());
    doors.erase(doors.begin(), doors.end());
    keys.erase(keys.begin(), keys.end());
}

void EntityManager::createPlayer(const Vector3f& pos, const Vector3f& dim, const float& speed) {
    players.emplace_back(std::make_unique<EntityPlayer>(device, pos, dim, speed));
}

void EntityManager::createDoor(const Vector3f& pos, const Vector3f& dim) {
    doors.emplace_back(std::make_unique<EntityDoor>(device, pos, dim));
}

void EntityManager::createKey(const Vector3f& pos, const Vector3f& dim) {
    keys.emplace_back(std::make_unique<EntityKey>(device, pos, dim));
}
void EntityManager::createCamera(const Vector3f& pos, const Vector3f& target) {
    cameras.emplace_back(std::make_unique<EntityCamera>(device, pos, target));
}
