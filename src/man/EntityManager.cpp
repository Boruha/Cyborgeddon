#include <man/EntityManager.hpp>
#include <algorithm>

/*		Destructor		*/
void EntityManager::cleanVectors() {
	entities.clear();
}

/*		Init - Update	*/
void EntityManager::init() {
	entities.reserve(256);

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
	checkEntitiesToDestroy();
}

// TODO: en los managers no debe haber logica. Revisar sistema de input
// BORU: No se puede acceder al createBullet() desde input (al menos ahora). Lo ideal sería crear desde ahí obv.
// 		 si se consigue, la var. 'Shooting' se podrá elilminar.
void EntityManager::checkShooting() {
	if(player->shooting){
		createBullet(Vector3f(3));
		player->shooting = false;
	}
}

/*		DESTROY ENTITIES	*/


void EntityManager::checkEntitiesToDestroy() {
	auto it = entities.begin();

	while(it != entities.end()) {
		if (!(*it)->alive.alive)
			it = entities.erase(it);
		else
			++it;
	}
}


/*		CREATE ENTITIES		*/

void EntityManager::createPairPlayerCamera(const int& health, const Vector3f& pos, const Vector3f& dim, const float& speed, const Vector3f& posCamera) {
	auto e_player = std::make_unique<EntityPlayer>();
	auto e_camera = std::make_unique<EntityCamera>();

	Velocity& velocity = componentStorage.createVelocity(e_player->getType(), e_player->getID(), 1.f);

	Physics& physicsPlayer = componentStorage.createPhysics(e_player->getType(), e_player->getID(), pos + Vector3f(0, dim.y / 2, 0));
	Physics& physicsCamera = componentStorage.createPhysics(e_camera->getType(), e_camera->getID(), posCamera, physicsPlayer.velocity);

	BoundingBox& box = componentStorage.createBoundingBox(e_player->getType(), e_player->getID(), dim, physicsPlayer.position);

	SceneNode& playerNode = componentStorage.createSceneNode(device, physicsPlayer.position, physicsPlayer.rotation, box.dim, nullptr, "./img/textures/testing/testing_demon.jpg");
	CameraNode& cameraNode = componentStorage.createCameraNode(device, physicsCamera.position, physicsPlayer.position);

	e_player->velocity = &velocity;
	e_player->physics = &physicsPlayer;
	e_player->collider = &box;
	e_player->node = &playerNode;

	e_camera->physics = &physicsCamera;
	e_camera->node = &cameraNode;

	player = std::move(e_player);
	camera = std::move(e_camera);
}

void EntityManager::createWall(const Vector3f& pos, const Vector3f& dim) {
	auto wall = std::make_unique<EntityWall>();

	Transformable& transformable = componentStorage.createTransformable(wall->getType(), wall->getID(), pos + Vector3f(0, dim.y / 2, 0));
	BoundingBox& box = componentStorage.createBoundingBox(wall->getType(), wall->getID(), dim, transformable.position, true);
	SceneNode& node = componentStorage.createSceneNode(device, transformable.position, transformable.rotation, box.dim, nullptr, "./img/textures/testing/testing_wall.jpg");

	wall->transformable = &transformable;
	wall->collider = &box;
	wall->node = &node;

	entities.emplace_back(std::move(wall));
}

void EntityManager::createEnemy(const Vector3f& pos, const Vector3f& dim, const float& speed, const std::vector<Vector3f>& patrol) {
	auto enemy = std::make_unique<EntityEnemy>();

	Physics& physics = componentStorage.createPhysics(enemy->getType(), enemy->getID(), pos + Vector3f(0, dim.y / 2, 0), Vector3f());
	Velocity& velocity = componentStorage.createVelocity(enemy->getType(), enemy->getID(), speed);
	BoundingBox& box = componentStorage.createBoundingBox(enemy->getType(), enemy->getID(), dim, physics.position, true);
	AI& ai = componentStorage.createAI(enemy->getType(), enemy->getID(), patrol);
	SceneNode& node = componentStorage.createSceneNode(device, physics.position, physics.rotation, box.dim, nullptr, "./img/textures/testing/testing_enemy.png");

	enemy->physics = &physics;
	enemy->velocity = &velocity;
	enemy->collider = &box;
	enemy->ai = &ai;
	enemy->node = &node;

	entities.emplace_back(std::move(enemy));
}

void EntityManager::createFloor(const char* tex, const Vector3f& pos, const Vector3f& dim) {
	auto floor = std::make_unique<EntityFloor>();

	Transformable& transformable = componentStorage.createTransformable(floor->getType(), floor->getID(), pos + Vector3f(0, dim.y / 2, 0));
 	SceneNode& node = componentStorage.createSceneNode(device, transformable.position, transformable.rotation, dim, nullptr, tex);

 	floor->transformable = &transformable;
 	floor->node = &node;

 	entities.emplace_back(std::move(floor));
}

void EntityManager::createBullet(const Vector3f& dim) {
	auto bullet = std::make_unique<EntityBullet>();

	Physics& physics = componentStorage.createPhysics(bullet->getType(), bullet->getID(), player->physics->position, Vector3f().getXZfromRotationY(player->physics->rotation.y) * 10.f, player->physics->rotation);
	physics.position += physics.velocity; // la bala no sale de dentro del player, sino desde delante de el

	BoundingBox& box = componentStorage.createBoundingBox(bullet->getType(), bullet->getID(), dim, physics.position);

	SceneNode& node = componentStorage.createSceneNode(device, physics.position, physics.rotation, box.dim, nullptr, nullptr);

	player->mode ? node.setTexture("./img/textures/testing/testing_angel.jpg") : node.setTexture("./img/textures/testing/testing_demon.jpg");

	bullet->physics = &physics;
	bullet->collider = &box;
	bullet->node = &node;
	bullet->dmgType = player->mode;

	entities.emplace_back(std::move(bullet));
}

void EntityManager::createPairKeyDoor(const Vector3f& keyPos, const Vector3f& keyDim, const Vector3f& doorPos, const Vector3f& doorDim) {
	auto door = std::make_unique<EntityDoor>();
	auto key = std::make_unique<EntityKey>();

	Transformable& transformableDoor = componentStorage.createTransformable(door->getType(), door->getID(), doorPos + Vector3f(0, doorDim.y / 2, 0));
	Transformable& transformableKey = componentStorage.createTransformable(key->getType(), key->getID(), keyPos + Vector3f(0, keyDim.y / 2, 0));

	BoundingBox& boxDoor = componentStorage.createBoundingBox(door->getType(), door->getID(), doorDim, transformableDoor.position, true);
	BoundingBox& boxKey = componentStorage.createBoundingBox(key->getType(), key->getID(), keyDim, transformableKey.position);

	SceneNode& nodeDoor = componentStorage.createSceneNode(device, transformableDoor.position, transformableDoor.rotation, boxDoor.dim, nullptr, "./img/textures/testing/testing_door.png");
	SceneNode& nodeKey = componentStorage.createSceneNode(device, transformableKey.position, transformableKey.rotation, boxKey.dim, nullptr, "./img/textures/testing/testing_key.png");

	Lock& lock = componentStorage.createLock(door->getType(), door->getID()); // la cerradura pertenece a la puerta, NO a la llave

	door->lock = &lock;
	door->transformable = &transformableDoor;
	door->collider = &boxDoor;
	door->node = &nodeDoor;

	key->lock = &lock;
	key->transformable = &transformableKey;
	key->collider = &boxKey;
	key->node = &nodeKey;

	entities.emplace_back(std::move(door));
	entities.emplace_back(std::move(key));
}
