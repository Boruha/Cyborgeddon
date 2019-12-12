#include <man/EntityManager.hpp>
#include <algorithm>

/*		Destructor		*/
void EntityManager::cleanVectors() {
	bullets.erase(bullets.begin(), bullets.end());
	keys.erase(keys.begin(), keys.end());
	doors.erase(doors.begin(), doors.end());
	enemies.erase(enemies.begin(), enemies.end());
	entities.erase(entities.begin(), entities.end());
}


/*		Init - Update	*/
void EntityManager::init()
{
    createCamera(Vector3f(0, 20, -50));

    createPlayer(Vector3f(0,0,0), Vector3f(7.f));
	createEnemy(Vector3f(40, 0, 40));

    createDoor(Vector3f(20,0,10));
    createDoor(Vector3f(-25,0,5));
    createDoor(Vector3f(-20,0,15));

    createKey(0, Vector3f(0,0,10));
    createKey(1, Vector3f(20,0,0));
	createKey(2, Vector3f(30,0,0));
}

void EntityManager::update(){
    checkShooting();
	openDoor();
	takeKey();
    deleteBullet();
}

// TODO: en los managers no debe haber logica. Revisar sistema de input
void EntityManager::checkShooting(){
	if(entities[0]->shooting){
		createBullet(entities[0]->node.getPosition(), entities[0]->velocity.direction, entities[0]->mode);
		entities[0]->shooting = false;
	}
}




/*		DESTROY ENTITIES	*/

void EntityManager::takeKey(){
	keys.erase(std::remove_if(keys.begin(), keys.end(), [](auto const& key) { return key->taken; }), keys.end());
}

void EntityManager::openDoor() {
	doors.erase(std::remove_if(doors.begin(), doors.end(), [](auto const& door) { return door->open; }), doors.end());
}

void EntityManager::deleteBullet(){
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](auto const& bullet) { return bullet->dead; }), bullets.end());
}




/*		CREATE ENTITIES		*/

void EntityManager::createPlayer(const Vector3f& pos, const Vector3f& dim, const float& speed) {
	entities.emplace_back(std::make_unique<EntityPlayer>(device, pos + Vector3f(0, dim.y / 2, 0), dim, speed));
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
void EntityManager::createKey(const int& type, const Vector3f& pos, const Vector3f& dim) {
    keys.emplace_back(std::make_unique<EntityKey>(device, pos + Vector3f(0, dim.y / 2, 0), dim, type));
}

// TODO: que la bala dependa de la orientacion del player y no de su direccion
void EntityManager::createBullet(const Vector3f& pos, Vector3f dir, const bool& type, const Vector3f& dim) {
	if (dir == 0)
		dir.z = 1; // para que la bala nunca se quede parada y por lo menos avance
    bullets.emplace_back(std::make_unique<EntityBullet>(device, pos + Vector3f(0, dim.y / 2, 0), dim, dir, type));
}