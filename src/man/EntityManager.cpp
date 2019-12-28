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
    createPairPlayerCamera(10, Vector3f(), Vector3f(6.f), 1.f, Vector3f(10, 90, -30));

	//------------ Creacion del escenario para las Christmas ------------------------------------------
	createFloor("./img/textures/testing/testing_controls.jpg",Vector3f(0,0,5), Vector3f(60,0,35)); //Controls
    createFloor("./img/textures/testing/testing_tips.jpg",Vector3f(-2,0,-27), Vector3f(45,0,15)); //Tips

    // Doors and keys

    createPairKeyDoor(Vector3f(0,0,60), Vector3f(3),Vector3f(-37,0,90), Vector3f(2,20,10));
    createPairKeyDoor(Vector3f(-70,0,90), Vector3f(3), Vector3f(37,0,0), Vector3f(2,20,10));
    createPairKeyDoor(Vector3f(70,0,0), Vector3f(3), Vector3f(-37,0,0), Vector3f(2,20,10));
    createPairKeyDoor(Vector3f(-70,0,0), Vector3f(3), Vector3f(37,0,190), Vector3f(2,20,10));
    createPairKeyDoor(Vector3f(70,0,190), Vector3f(3), Vector3f(-37,0,190), Vector3f(2,20,10));
    createPairKeyDoor(Vector3f(-70,0,190), Vector3f(3), Vector3f(37,0,90), Vector3f(2,20,10));
    createPairKeyDoor(Vector3f(70,0,90), Vector3f(3), Vector3f(152.5,0,300), Vector3f(45,10,10));
    createPairKeyDoor(Vector3f(158,0,320), Vector3f(3), Vector3f(-180,0,272.5), Vector3f(10,10,45));


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
		createBullet(Vector3f(3));
		player->shooting = false;
	}
}


/*		DESTROY ENTITIES	*/

void EntityManager::takeKey() {
	keys.erase(std::remove_if(keys.begin(), keys.end(), [](auto const& key) { return !key->alive.alive; }), keys.end());
}

void EntityManager::openDoor() {
	doors.erase(std::remove_if(doors.begin(), doors.end(), [](auto const& door) { return !door->alive.alive; }), doors.end());
}

void EntityManager::deleteBullet() {
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](auto const& bullet) { return !bullet->alive.alive; }), bullets.end());
}

void EntityManager::killEnemy() {
    enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](auto const& enemy) { return !enemy->alive.alive; }), enemies.end());
}

void EntityManager::killPlayer() {
}


/*		CREATE ENTITIES		*/

void EntityManager::createPairPlayerCamera(const int& health, const Vector3f& pos, const Vector3f& dim, const float& speed, const Vector3f& posCamera) {
	Transformable& transformablePlayer = componentStorage.createTransformable(pos + Vector3f(0, dim.y / 2, 0));
	Transformable& transformableCamera = componentStorage.createTransformable(posCamera);

	Velocity& velocity = componentStorage.createVelocity(1.f);

	SceneNode& playerNode = componentStorage.createSceneNode(device, transformablePlayer.position, transformablePlayer.rotation, dim, nullptr, "./img/textures/testing/testing_demon.jpg");
	CameraNode& cameraNode = componentStorage.createCameraNode(device, transformableCamera.position, transformablePlayer.position);

	player = std::make_unique<EntityPlayer>(transformablePlayer, velocity, dim, playerNode);
	camera = std::make_unique<EntityCamera>(transformableCamera, velocity, cameraNode);
}

void EntityManager::createWall(const Vector3f& pos, const Vector3f& dim) {
	Transformable& transformable = componentStorage.createTransformable(pos + Vector3f(0, dim.y / 2, 0));
	SceneNode& node = componentStorage.createSceneNode(device, transformable.position, transformable.rotation, dim, nullptr, "./img/textures/testing/testing_wall.jpg");

	walls.emplace_back(std::make_unique<EntityWall>(transformable, dim,node));
}

void EntityManager::createEnemy(const Vector3f& pos, const Vector3f& dim, const float& speed, const std::vector<Vector3f>& patrol) {
	Transformable& transformable = componentStorage.createTransformable(pos + Vector3f(0, dim.y / 2, 0));
	Velocity& velocity = componentStorage.createVelocity(speed);
	AI& ai = componentStorage.createAI(patrol);
	SceneNode& node = componentStorage.createSceneNode(device, transformable.position, transformable.rotation, dim, nullptr, "./img/textures/testing/testing_enemy.png");

	enemies.emplace_back(std::make_unique<EntityEnemy>(transformable, velocity, dim, ai, node));
}

void EntityManager::createDoor(const Lock& lock, const Vector3f& pos, const Vector3f& dim) {
    Transformable& transformable = componentStorage.createTransformable(pos + Vector3f(0, dim.y / 2, 0));
    SceneNode& node = componentStorage.createSceneNode(device, transformable.position, transformable.rotation, dim, nullptr, "./img/textures/testing/testing_door.png");

    doors.emplace_back(std::make_unique<EntityDoor>(transformable, dim, lock, node));
}
void EntityManager::createKey(const Lock& lock, const Vector3f& pos, const Vector3f& dim) {
	Transformable& transformable = componentStorage.createTransformable(pos + Vector3f(0, dim.y / 2, 0));
	SceneNode& node = componentStorage.createSceneNode(device, transformable.position, transformable.rotation, dim, nullptr, "./img/textures/testing/testing_key.png");

	keys.emplace_back(std::make_unique<EntityKey>(transformable, dim, lock, node));
}

void EntityManager::createFloor(const char* tex, const Vector3f& pos, const Vector3f& dim) {
	Transformable& transformable = componentStorage.createTransformable(pos + Vector3f(0, dim.y / 2, 0));
 	SceneNode& node = componentStorage.createSceneNode(device, transformable.position, transformable.rotation, dim, nullptr, tex);

 	floor.emplace_back(std::make_unique<EntityFloor>(tex, transformable, node));
}

void EntityManager::createBullet(const Vector3f& dim) {
	Transformable& transformable = componentStorage.createTransformable(player->transformable->position);
	Velocity& velocity = componentStorage.createVelocity(Vector3f().getXZfromRotationY(player->transformable->rotation.y), 10.f);
	SceneNode& node = componentStorage.createSceneNode(device, transformable.position, transformable.rotation, dim, nullptr, nullptr);

	player->mode ? node.setTexture("./img/textures/testing/testing_angel.jpg") : node.setTexture("./img/textures/testing/testing_demon.jpg");

	bullets.emplace_back(std::make_unique<EntityBullet>(transformable, velocity, dim, player->mode, node));
}

void EntityManager::createPairKeyDoor(const Vector3f &keyPos, const Vector3f &keyDim, const Vector3f &doorPos, const Vector3f &doorDim) {
	const Lock& lock = componentStorage.createLock();

	createKey(lock, keyPos, keyDim);
	createDoor(lock, doorPos, doorDim);

	player->my_keys.emplace_back(false);
}
