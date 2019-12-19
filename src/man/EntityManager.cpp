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
	createCamera(Vector3f(10, 90, -30));

	//------------ Creacion del escenario para las Christmas ------------------------------------------

	//Pasillo inicial

	    //Derecha
        createWall(Vector3f(40,0,-27.5), Vector3f(10,10,45));
        createWall(Vector3f(40,0,45), Vector3f(10,10,80));
        createWall(Vector3f(40,0,140), Vector3f(10,10,90));
        createWall(Vector3f(40,0,222.5), Vector3f(10,10,55));

        //Izquierda
        createWall(Vector3f(-40,0,-27.5), Vector3f(10,10,45));
        createWall(Vector3f(-40,0,45), Vector3f(10,10,80));
        createWall(Vector3f(-40,0,140), Vector3f(10,10,90));
        createWall(Vector3f(-40,0,222.5), Vector3f(10,10,55));

        createWall(Vector3f(0,0,-45), Vector3f(70,10,10));      //Cierre inferior
        createKey(0, Vector3f(0,0,-30));

        //Salas del pasillo

            //Sala 1
            createWall(Vector3f(75,0,-30), Vector3f(60,10,10));    //Abajo
            createWall(Vector3f(75,0,30), Vector3f(60,10,10));    //Arriba
            createWall(Vector3f(100,0,0), Vector3f(10,10,50));    //Cierre
            createDoor(1, Vector3f(37,0,0), Vector3f(2,20,10));
            createKey(2, Vector3f(70,0,0));

            //Sala 2
            createWall(Vector3f(-75,0,-30), Vector3f(60,10,10));    //Abajo
            createWall(Vector3f(-75,0,30), Vector3f(60,10,10));    //Arriba
            createWall(Vector3f(-100,0,0), Vector3f(10,10,50));    //Cierre
            createDoor(2, Vector3f(-37,0,0), Vector3f(2,20,10));
            createKey(3, Vector3f(-70,0,0));

            //Sala 3
            createWall(Vector3f(75,0,60), Vector3f(60,10,10));    //Abajo
            createWall(Vector3f(75,0,120), Vector3f(60,10,10));    //Arriba
            createWall(Vector3f(100,0,90), Vector3f(10,10,50));    //Cierre
            createDoor(5, Vector3f(37,0,90), Vector3f(2,20,10));
            createKey(6, Vector3f(70,0,90));

            //Sala 4
            createWall(Vector3f(-75,0,60), Vector3f(60,10,10));    //Abajo
            createWall(Vector3f(-75,0,120), Vector3f(60,10,10));    //Arriba
            createWall(Vector3f(-100,0,90), Vector3f(10,10,50));    //Cierre
            createDoor(0, Vector3f(-37,0,90), Vector3f(2,20,10));
            createKey(1, Vector3f(-70,0,90));

            //Sala 5
            createWall(Vector3f(-75,0,160), Vector3f(60,10,10));    //Abajo
            createWall(Vector3f(-75,0,220), Vector3f(60,10,10));    //Arriba
            createWall(Vector3f(-100,0,190), Vector3f(10,10,50));    //Cierre
            createDoor(3, Vector3f(37,0,190), Vector3f(2,20,10));
            createKey(4, Vector3f(70,0,190));

            //Sala 6
            createWall(Vector3f(75,0,160), Vector3f(60,10,10));    //Abajo
            createWall(Vector3f(75,0,220), Vector3f(60,10,10));    //Arriba
            createWall(Vector3f(100,0,190), Vector3f(10,10,50));    //Cierre
            createDoor(4, Vector3f(-37,0,190), Vector3f(2,20,10));
            createKey(5, Vector3f(-70,0,190));

    //Pasillo Horizontal
    createWall(Vector3f(-115,0,245), Vector3f(140,10,10));    //Inferior izda
    createWall(Vector3f(115,0,245), Vector3f(140,10,10));     //Inderior dcha
    createWall(Vector3f(-27.5,0,300), Vector3f(315,10,10));   //Superior
    createWall(Vector3f(180,0,277.5), Vector3f(10,10,55));  //Derecha
    createDoor(7, Vector3f(-180,0,272.5), Vector3f(10,10,45));  //Puerta patrulla

    //Sala llave principal
    createDoor(6, Vector3f(152.5,0,300), Vector3f(45,10,10));       //Puerta llave principal
    createKey(7, Vector3f(158,0,320));
    createWall(Vector3f(200,0,300), Vector3f(30,10,10));
    createWall(Vector3f(210,0,345), Vector3f(10,10,80));
    createWall(Vector3f(100,0,345), Vector3f(10,10,80));
    createWall(Vector3f(157.5,0,380), Vector3f(105,10,10));

    //Zona patrulla
    createWall(Vector3f(-180,0,227.5), Vector3f(10,10,45)); //Inf der
    createWall(Vector3f(-180,0,327.5), Vector3f(10,10,45)); //Sup der
    createWall(Vector3f(-262.5,0,345), Vector3f(160,10,10)); //Sup
    createWall(Vector3f(-262.5,0,210), Vector3f(160,10,10)); //Inf
    createWall(Vector3f(-347.5,0,277.5), Vector3f(10,10,145)); //Izq
    createWall(Vector3f(-265,0,277.5), Vector3f(60,20,55)); //Pilar
    //------------------------------------  END MAPA  ---------------------------------------------------------------

    createPlayer(10, Vector3f(0,0,0), Vector3f(6.f));

	createEnemy(Vector3f(40, 0, 40));
    createEnemy(Vector3f(80, 0, 80));
    createEnemy(Vector3f(-40, 0, 40));
    createEnemy(Vector3f(-80, 0, 80));

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

void EntityManager::createPlayer(const int& health, const Vector3f& pos, const Vector3f& dim, const float& speed) {
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

void EntityManager::createBullet(const Vector3f& pos, const Vector3f& dir, const bool& type, const Vector3f& dim) {
    bullets.emplace_back(std::make_unique<EntityBullet>(device, pos, dim, dir, type));
}