#include <man/EntityManager.hpp>
#include <algorithm>

void EntityManager::init()
{
    createPlayer(Vector3f(0,0,100));

    createEnemy(Vector3f(0, 0, 50));

    createCamera(Vector3f(0, 20, -50));

    createDoor(Vector3f(20,0,10));
    createDoor(Vector3f(-25,0,5));
    createDoor(Vector3f(-20,0,15));

    createKey(Vector3f(0,0,10));
    createKey(Vector3f(20,0,0));
	createKey();
}

void EntityManager::takeKey(){
	keys.erase(std::remove_if(keys.begin(), keys.end(), [](auto const& key) { return key->taken; }), keys.end());
}

void EntityManager::openDoor(){
	doors.erase(std::remove_if(doors.begin(), doors.end(), [](auto const& door) { return door->open; }), doors.end());
}

void EntityManager::cleanVectors() {
	entities.erase(entities.begin(), entities.end());
	doors.erase(doors.begin(), doors.end());
	keys.erase(keys.begin(), keys.end());
	delete player; // TODO: eliminar esto cuanto antes
}

void EntityManager::createPlayer(const Vector3f& pos, const Vector3f& dim, const float& speed) {
	player = new EntityPlayer(device, pos + Vector3f(0, dim.y / 2, 0), dim, speed);
	// TODO: eliminar esto cuanto antes
}

void EntityManager::createCamera(const Vector3f& pos, const Vector3f& target) {
    camera.cameraNode.setPosition(camera.transformable.position = pos);
    camera.cameraNode.setTarget(camera.camera.target = target);
}

void EntityManager::createEnemy(const Vector3f& pos, const Vector3f& dim, const float& speed) {
	enemies.emplace_back(std::make_unique<EntityEnemy>(device, pos + Vector3f(0, dim.y / 2, 0), dim, speed));
}

void EntityManager::createDoor(const Vector3f& pos, const Vector3f& dim) {
    doors.emplace_back(std::make_unique<EntityDoor>(device, pos + Vector3f(0, dim.y / 2, 0), dim));
}

void EntityManager::createKey(const Vector3f& pos, const Vector3f& dim) {
    keys.emplace_back(std::make_unique<EntityKey>(device, pos + Vector3f(0, dim.y / 2, 0), dim));
}