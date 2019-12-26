#include <man/EntityManager.hpp>
#include <algorithm>

/*		Destructor		*/
void EntityManager::cleanVectors() {
	floor.clear();
	bullets.clear();
	keys.clear();
	doors.clear();
	enemies.clear();
	walls.clear();
}

/*		Init - Update	*/
void EntityManager::init(){
    createPlayer(10, Vector3f(0,0,0), Vector3f(6.f));
	createCamera(Vector3f(10, 90, -30));

	//------------ Creacion del escenario para las Christmas ------------------------------------------
	createFloor("./img/textures/testing/testing_controls.jpg",Vector3f(0,0,5), Vector3f(60,0,35)); //Controls
    createFloor("./img/textures/testing/testing_tips.jpg",Vector3f(-2,0,-27), Vector3f(45,0,15)); //Tips

    // Doors and keys

    createDoor(Vector3f(-37,0,90), Vector3f(2,20,10));
    createKey(doors.at(doors.size() - 1)->lock, Vector3f(0,0,60));

    createDoor(Vector3f(37,0,0), Vector3f(2,20,10));
	createKey(doors.at(doors.size() - 1)->lock, Vector3f(-70,0,90));

	createDoor(Vector3f(-37,0,0), Vector3f(2,20,10));
	createKey(doors.at(doors.size() - 1)->lock, Vector3f(70,0,0));

	createDoor(Vector3f(37,0,190), Vector3f(2,20,10));
	createKey(doors.at(doors.size() - 1)->lock, Vector3f(-70,0,0));

	createDoor(Vector3f(-37,0,190), Vector3f(2,20,10));
	createKey(doors.at(doors.size() - 1)->lock, Vector3f(70,0,190));

	createDoor(Vector3f(37,0,90), Vector3f(2,20,10));
	createKey(doors.at(doors.size() - 1)->lock, Vector3f(-70,0,190));

	createDoor(Vector3f(152.5,0,300), Vector3f(45,10,10));       //Puerta llave principal
	createKey(doors.at(doors.size() - 1)->lock, Vector3f(70,0,90));

	createDoor(Vector3f(-180,0,272.5), Vector3f(10,10,45));  //Puerta patrulla
    createKey(doors.at(doors.size() - 1)->lock, Vector3f(158,0,320));

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

        //Salas del pasillo

            //Sala 1
            createWall(Vector3f(75,0,-30), Vector3f(60,10,10));    //Abajo
            createWall(Vector3f(75,0,30), Vector3f(60,10,10));    //Arriba
            createWall(Vector3f(100,0,0), Vector3f(10,10,50));    //Cierre

            //Sala 2
            createWall(Vector3f(-75,0,-30), Vector3f(60,10,10));    //Abajo
            createWall(Vector3f(-75,0,30), Vector3f(60,10,10));    //Arriba
            createWall(Vector3f(-100,0,0), Vector3f(10,10,50));    //Cierre

            //Sala 3
            createWall(Vector3f(75,0,60), Vector3f(60,10,10));    //Abajo
            createWall(Vector3f(75,0,120), Vector3f(60,10,10));    //Arriba
            createWall(Vector3f(100,0,90), Vector3f(10,10,50));    //Cierre

            //Sala 4
            createWall(Vector3f(-75,0,60), Vector3f(60,10,10));    //Abajo
            createWall(Vector3f(-75,0,120), Vector3f(60,10,10));    //Arriba
            createWall(Vector3f(-100,0,90), Vector3f(10,10,50));    //Cierre

            //Sala 5
            createWall(Vector3f(-75,0,160), Vector3f(60,10,10));    //Abajo
            createWall(Vector3f(-75,0,220), Vector3f(60,10,10));    //Arriba
            createWall(Vector3f(-100,0,190), Vector3f(10,10,50));    //Cierre

            //Sala 6
            createWall(Vector3f(75,0,160), Vector3f(60,10,10));    //Abajo
            createWall(Vector3f(75,0,220), Vector3f(60,10,10));    //Arriba
            createWall(Vector3f(100,0,190), Vector3f(10,10,50));    //Cierre

    //Pasillo Horizontal
    createWall(Vector3f(-115,0,245), Vector3f(140,10,10));    //Inferior izda
    createWall(Vector3f(115,0,245), Vector3f(140,10,10));     //Inderior dcha
    createWall(Vector3f(-27.5,0,300), Vector3f(315,10,10));   //Superior
    createWall(Vector3f(180,0,277.5), Vector3f(10,10,55));  //Derecha

    //Sala llave principal
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


    std::vector<Vector3f> patrol_1 = { Vector3f(-160, 0, 270), Vector3f(-160, 0, 270), Vector3f(-160, 0, 270), Vector3f(-160, 0, 270) };
    std::vector<Vector3f> patrol_2 = { Vector3f(0, 0, 200), Vector3f(0, 0, 200), Vector3f(0, 0, 200), Vector3f(0, 0, 200) };
    std::vector<Vector3f> patrol_3 = { Vector3f(0, 0, 100), Vector3f(0, 0, 100), Vector3f(0, 0, 100), Vector3f(0, 0, 100) };
    std::vector<Vector3f> patrol_4 = { Vector3f(120, 0, 270), Vector3f(120, 0, 270), Vector3f(120, 0, 270), Vector3f(120, 0, 270) };
    std::vector<Vector3f> patrol_5 = { Vector3f(-315, 0, 230), Vector3f(-315, 0, 320), Vector3f(-210, 0, 320), Vector3f(-210, 0, 230) };


	createEnemy(patrol_1[0], Vector3f(8), 0.6f, patrol_1);
	createEnemy(patrol_2[0], Vector3f(8), 0.5f, patrol_2);
	createEnemy(patrol_3[0], Vector3f(8), 0.4f, patrol_3);
	createEnemy(patrol_4[0], Vector3f(8), 0.3f, patrol_4);
	createEnemy(patrol_5[0], Vector3f(8), 0.3f, patrol_5);
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
	if(player->shooting){
		createBullet(player->node.getPosition(), Vector3f().getXZfromRotationY(player->transformable.rotation.y), player->mode);
		player->shooting = false;
	}
}

/*		DESTROY ENTITIES	*/

void EntityManager::takeKey(){
    //CONDICION PARA QUE MUERA ----> TYPE < 0
	keys.erase(std::remove_if(keys.begin(), keys.end(), [](auto const& key) { return !key->alive; }), keys.end());
}

void EntityManager::openDoor() {
    //CONDICION PARA QUE MUERA ----> TYPE < 0
	doors.erase(std::remove_if(doors.begin(), doors.end(), [](auto const& door) { return !door->alive; }), doors.end());
}

void EntityManager::deleteBullet(){
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](auto const& bullet) { return !bullet->alive; }), bullets.end());
}

void EntityManager::killEnemy(){
    //CONDICION PARA QUE MUERA ----> State < 0
    enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](auto const& enemy) { return !enemy->alive; }), enemies.end());
}


void EntityManager::killPlayer(){
    //CONDICION PARA QUE MUERA ----> HP < 0
}


/*		CREATE ENTITIES		*/

void EntityManager::createPlayer(const int& health, const Vector3f& pos, const Vector3f& dim, const float& speed) {
	player = std::make_unique<EntityPlayer>(device,health,  pos + Vector3f(0, dim.y / 2, 0), dim, speed);
}

void EntityManager::createWall(const Vector3f& pos, const Vector3f& dim) {
    walls.emplace_back(std::make_unique<EntityWall>(device,  pos + Vector3f(0, dim.y / 2, 0), dim));
}

void EntityManager::createCamera(const Vector3f& pos) {
    camera = std::make_unique<EntityCamera>(device, pos, player->transformable.position);
}

void EntityManager::createEnemy(const Vector3f& pos, const Vector3f& dim, const float& speed, const std::vector<Vector3f>& patrol) {
	enemies.emplace_back(std::make_unique<EntityEnemy>(device, pos + Vector3f(0, dim.y / 2, 0), dim, speed, patrol));
}

void EntityManager::createDoor(const Vector3f& pos, const Vector3f& dim) {
    doors.emplace_back(std::make_unique<EntityDoor>(device, pos + Vector3f(0, dim.y / 2, 0), dim ));
}
void EntityManager::createKey(const Lock& lock, const Vector3f& pos, const Vector3f& dim) {
    keys.emplace_back(std::make_unique<EntityKey>(device, lock, pos + Vector3f(0, dim.y / 2, 0), dim));
	player->my_keys.push_back(false);
}

void EntityManager::createFloor(const char* tex, const Vector3f& pos, const Vector3f& dim) {
    floor.emplace_back(std::make_unique<EntityFloor>(device, tex, pos + Vector3f(0, dim.y / 2, 0), dim ));
}

void EntityManager::createBullet(const Vector3f& pos, const Vector3f& dir, const bool& type, const Vector3f& dim) {
    bullets.emplace_back(std::make_unique<EntityBullet>(device, pos, dim, dir, type));
}