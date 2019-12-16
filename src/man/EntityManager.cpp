#include <man/EntityManager.hpp>
#include <algorithm>

/*		Destructor		*/
void EntityManager::cleanVectors() {
	bullets.erase(bullets.begin(), bullets.end());
	keys.erase(keys.begin(), keys.end());
	doors.erase(doors.begin(), doors.end());
	enemies.erase(enemies.begin(), enemies.end());
	entities.erase(entities.begin(), entities.end());
    walls.erase(walls.begin(), walls.end());
}


/*		Init - Update	*/
void EntityManager::init()
{
    createCamera(Vector3f(0, 200, -50));

    createWall(Vector3f(40,0,100), Vector3f(10,10,300));
    createWall(Vector3f(-40,0,100), Vector3f(10,10,300));
    createWall(Vector3f(0,0,-45), Vector3f(70,10,10));
    createWall(Vector3f(-80,0,245), Vector3f(70,10,10));
    createWall(Vector3f(80,0,245), Vector3f(70,10,10));

    createPlayer(10, Vector3f(0,0,0), Vector3f(7.f));
	createEnemy(Vector3f(40, 0, 40));
    createEnemy(Vector3f(80, 0, 80));
    createEnemy(Vector3f(-40, 0, 40));
    createEnemy(Vector3f(-80, 0, 80));

    createDoor(0, Vector3f(20,0,0));
    createDoor(1, Vector3f(-20,0,0));
    createDoor(2, Vector3f(-20,0,15));

    createKey(0, Vector3f(0,0,10));
    createKey(1, Vector3f(20,0,0));
	createKey(2, Vector3f(30,0,0));
}

void EntityManager::update(){
    checkShooting();
	openDoor();
	takeKey();
    deleteBullet();
    killEnemy();
    //killPlayer();             //De momento comentada porque si se elimina el unico player que hay sigue haciendo comprobaciones con player y saca segmentation fault
}

// TODO: en los managers no debe haber logica. Revisar sistema de input
// BORU: No se puede acceder al createBullet() desde input (al menos ahora). Lo ideal sería crear desde ahí obv.
// 		 si se consigue, la var. 'Shooting' se podrá elilminar.
void EntityManager::checkShooting(){
	if(entities[0]->shooting){
		createBullet(entities[0]->node.getPosition(), entities[0]->transformable.rotation, entities[0]->mode);
		entities[0]->shooting = false;
	}
}




/*		DESTROY ENTITIES	*/

void EntityManager::takeKey(){
    //CONDICION PARA QUE MUERA ----> TYPE < 0
	keys.erase(std::remove_if(keys.begin(), keys.end(), [](auto const& key) { return key->type < 0; }), keys.end());
}

void EntityManager::openDoor() {
    //CONDICION PARA QUE MUERA ----> TYPE < 0
	doors.erase(std::remove_if(doors.begin(), doors.end(), [](auto const& door) { return door->type < 0; }), doors.end());
}

void EntityManager::deleteBullet(){
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](auto const& bullet) { return bullet->dead; }), bullets.end());
}

void EntityManager::killEnemy(){
    //CONDICION PARA QUE MUERA ----> State < 0
    enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](auto const& enemy) { return enemy->ai_state < 0; }), enemies.end());
}


void EntityManager::killPlayer(){
    //CONDICION PARA QUE MUERA ----> HP < 0
    entities.erase(std::remove_if(entities.begin(), entities.end(), [](auto const& player) { return player->health <= 0; }), entities.end());
}




/*		CREATE ENTITIES		*/

void EntityManager::createPlayer(int health, const Vector3f& pos, const Vector3f& dim, const float& speed) {
	entities.emplace_back(std::make_unique<EntityPlayer>(device,health,  pos + Vector3f(0, dim.y / 2, 0), dim, speed));
}

void EntityManager::createWall(const Vector3f& pos, const Vector3f& dim) {
    walls.emplace_back(std::make_unique<EntityWall>(device,  pos + Vector3f(0, dim.y / 2, 0), dim));
}

void EntityManager::createCamera(const Vector3f& pos, const Vector3f& target) {
    camera.cameraNode.setPosition(camera.transformable.position = pos);
    camera.cameraNode.setTarget(camera.camera.target = target);
}

void EntityManager::createEnemy(const Vector3f& pos, const Vector3f& dim, const float& speed) {
	enemies.emplace_back(std::make_unique<EntityEnemy>(device, pos + Vector3f(0, dim.y / 2, 0), dim, speed));
}

void EntityManager::createDoor(const int& type, const Vector3f& pos, const Vector3f& dim) {
    doors.emplace_back(std::make_unique<EntityDoor>(device, type, pos + Vector3f(0, dim.y / 2, 0), dim ));
}
void EntityManager::createKey(const int& type, const Vector3f& pos, const Vector3f& dim) {
    keys.emplace_back(std::make_unique<EntityKey>(device, type, pos + Vector3f(0, dim.y / 2, 0), dim));
}

// TODO: que la bala dependa de la orientacion del player y no de su direccion
void EntityManager::createBullet(const Vector3f& pos, Vector3f dir, const bool& type, const Vector3f& dim) {
	//if (dir == 0)
		//dir.y = 1; // disparamos al aire por los loles (y para que la bala no se quede parada)

    bullets.emplace_back(std::make_unique<EntityBullet>(device, pos, dim, dir, type));
}