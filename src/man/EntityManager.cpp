#include <man/EntityManager.hpp>

#include <algorithm>

#include <SunlightEngine/SceneNode.hpp>
#include <SunlightEngine/CameraNode.hpp>

#include <util/ComponentConstants.hpp>
#include <util/TexturePaths.hpp>

/*		Init - Update	*/
void EntityManager::init() {
	createLevel();	// de momento se crea asi sin mas, pero habra que utilizar un fichero de texto para generar el nivel
}

void EntityManager::initData(const int maxEntities, const int maxToDelete, const int maxComponents) {
	cleanData();								// si no es la primera vez que llamamos a esta funcion, hay que limpiar vectores, reiniciar variables...
	entities.reserve(maxEntities);				// reservamos memoria para la cantidad maxima de entidades esperada
	toDelete.reserve(maxToDelete);				// lo mismo para la cantidad maxima de entidades que pueden morir en una sola iteracion del juego
	componentStorage.initData(maxComponents);	// reservamos (de momento la misma) memoria para los vectores que tendran los componentes
	graph.reserve(5);
	createMapNode();
}

bool EntityManager::update(){
	checkShooting();

	if (!toDelete.empty())			// si hay entidades que "matar"
		killEntities();				// las matamos

	if (entitiesLeftToDelete > 0) {
		moveDeadEntities();				// las entidades muertas se van moviendo hacia el final
		removeEntities();				// las entidades que estan al final son eliminadas del vector
	}

	return checkVictory();
}

// TODO: en los managers no debe haber logica. Revisar sistema de input
// BORU: No se puede acceder al createBullet() desde input (al menos ahora). Lo ideal sería crear desde ahí obv.
// 		 si se consigue, la var. 'Shooting' se podrá elilminar.
void EntityManager::checkShooting() {
	if(player->characterData->attacking){
		createBullet(Vector3f(0.5, 0, player->collider->dim.z));
		player->characterData->attacking = false;
		player->characterData->currentAttackingCooldown = player->characterData->attackingCooldown;
	}
}

/*		DESTROY ENTITIES	*/

// aqui recibimos los IDS de las entidades que queremos destruir
void EntityManager::addToDestroy(std::size_t ID) {
	for (const auto& id : toDelete)
		if (id == ID)
			return;

	if (std::find(toDelete.begin(), toDelete.end(), ID) == toDelete.end()) { 	// si no existe el elemento
		toDelete.emplace_back(ID);
		std::sort(toDelete.begin(), toDelete.end(), std::less<>());	// ordenamos de forma ascendente (apenas tiene coste, el vector no ocupa mas de 8 * size_t de momento)
		++entitiesLeftToDelete;															// actualizamos el numero de entidades que nos faltan por eliminar
	}
}

void EntityManager::killEntities() {
	// recorremos el array de entidades en orden ascendente
	for (auto [e, d] = std::tuple{entities.begin(), toDelete.begin()}; e != entities.end() && d != toDelete.end(); ++e) {
		if (e->getID() != *d)
			continue;
		if(e->node){
			e->node->get()->removeFromScene();	// lo eliminamos de la escena
			e->node = nullptr;
		}										// si la entidad que queremos eliminar tiene nodo

		if (e->getType() == ENEMY)				// si era un enemigo
			--enemiesLeft;						// nos queda uno menos por matar

		e->makeUndefined();						// "matamos" a la entidad haciendo que su tipo y el de sus componentes sea UNDEFINED
		++d;									// actualizamos la posicion del vector toDelete (antes no se actualiza, pues no habiamos encontrado la entidad)
	}
	// no hay que hacer nada mas para comprobar si toDelete alcanza todas las entidades porque tanto "entities" como "toDelete" estan ordenados ascendentemente

	toDelete.clear();							// al acabar de matar las entidades, limpipamos el vector toDelete
}


void EntityManager::moveDeadEntities() {
	for (auto e = entities.rbegin(); e != entities.rend(); ++e)				// recorremos el vector de entidades inversamente (asi evitamos que se nos queden elementos descolgados
		if ((e + 1) != entities.rend() && (e + 1)->getType() == UNDEFINED)	// si la proxima entidad es valida y esta "muerta" (type == undefined)
			std::iter_swap(e,(e + 1));										// entonces la cambiamos con la entidad actual

	// Con esto conseguimos que las entidades muertas avancen en el array con un coste muy bajo (un swap) en cada iteracion
	// permitiendo a otra funcion eliminar los elementos que se encuentren al final.
	// Si solo eliminamos los elementos del final nos ahorramos tener que mover los elementos que hay detras.
	// De esta manera, en lugar de eliminar el elemento numero 10 de un vector de 100, teniendo que mover los elementos
	// 11 - 100 una posicion atras en una sola iteracion, repartimos esa carga en todas las iteraciones moviendo los
	// elementos correspondientes una unica posicion. Cuando lleguen al final seran eliminados sin mover nada
}

void EntityManager::removeEntities() {
	while(entities.at(entities.size() - 1).getType() == UNDEFINED) {
		entities.erase(entities.end() - 1);
		--entitiesLeftToDelete;							// una entidad menos que borrar
	}
}

void EntityManager::cleanData() {
	Entity::resetIDManagementValue();		// reiniciamos ID

	entities.clear();						// limpiamos entidades
	toDelete.clear();						// limpiamos vector de entidades a borrar

	entitiesLeftToDelete = 0;				// reiniciamos valor
	enemiesLeft = 0;						// reiniciamos valor

	player = nullptr;						// reiniciamos valor
	camera = nullptr;						// reiniciamos valor

	componentStorage.cleanData();			// limpiamos el contenido de component storage
}

/*		CREATE ENTITIES		*/

void EntityManager::createPairPlayerCamera(const Vector3f& pos, const Vector3f& dim, const Vector3f& posCamera) {
	player = & entities.emplace_back(PLAYER);
	camera = & entities.emplace_back(CAMERA);

	player->velocity		= & componentStorage.createComponent(VELOCITY_TYPE, Velocity(player->getType(), player->getID(), PLAYER_SPEED, PLAYER_ACCELERATION));
	player->physics			= & componentStorage.createComponent(PHYSICS_TYPE, Physics(player->getType(), player->getID(), pos + Vector3f(0, dim.y / 2, 0), Vector3f(), Vector3f()));
	player->collider		= & componentStorage.createComponent(SPECIAL_BOUNDING_BOX_TYPE, BoundingBox(player->getType(), player->getID(), dim, player->physics->position, player->physics->velocity, true, DYNAMIC));
	player->characterData	= & componentStorage.createComponent(CHARACTER_DATA_TYPE, CharacterData(player->getType(), player->getID(), false, PLAYER_HEALTH, PLAYER_ATTACK_DAMAGE, PLAYER_ATTACKING_COOLDOWN));
	player->node			= & componentStorage.createNode(Sun::SceneNode(device, player->physics->position, player->physics->rotation, player->collider->dim, nullptr, DEMON_TEXTURE));

	player->addComponent(*player->velocity);
	player->addComponent(*player->physics);
	player->addComponent(*player->collider);
	player->addComponent(*player->characterData);

	camera->physics		= & componentStorage.createComponent(PHYSICS_TYPE, Physics(camera->getType(), camera->getID(), posCamera, player->physics->velocity, Vector3f()));
	camera->node		= & componentStorage.createNode(Sun::CameraNode(device, player->physics->position, camera->physics->position));

	camera->addComponent(*camera->physics);
}

void EntityManager::createWall(const Vector3f& pos, const Vector3f& dim) {
	Entity& wall = entities.emplace_back(WALL);

	wall.physics	= & componentStorage.createComponent(PHYSICS_TYPE, Physics(wall.getType(), wall.getID(), pos + Vector3f(0, dim.y / 2, 0), Vector3f(), Vector3f()));
	wall.collider	= & componentStorage.createComponent(STATIC_BOUNDING_BOX_TYPE, BoundingBox(wall.getType(), wall.getID(), dim, wall.physics->position, wall.physics->velocity, false, STATIC));
	wall.node		= & componentStorage.createNode(Sun::SceneNode(device, wall.physics->position, wall.physics->rotation, wall.collider->dim, nullptr, WALL_TEXTURE));

	wall.addComponent(*wall.physics);
	wall.addComponent(*wall.collider);
}

void EntityManager::createEnemy(const Vector3f& pos, const Vector3f& dim, const std::vector<Vector3f>& patrol) {
	Entity& enemy = entities.emplace_back(ENEMY);

	enemy.physics		= & componentStorage.createComponent(PHYSICS_TYPE, Physics(enemy.getType(), enemy.getID(), pos + Vector3f(0, dim.y / 2, 0), Vector3f(), Vector3f()));
	enemy.velocity		= & componentStorage.createComponent(VELOCITY_TYPE, Velocity(enemy.getType(), enemy.getID(), ENEMY_SPEED, ENEMY_ACCELERATION));
	enemy.collider		= & componentStorage.createComponent(SPECIAL_BOUNDING_BOX_TYPE, BoundingBox(enemy.getType(), enemy.getID(), dim, enemy.physics->position, enemy.physics->velocity, false, STATIC));
	enemy.ai			= & componentStorage.createComponent(AI_TYPE, AI(enemy.getType(), enemy.getID(), patrol));
	enemy.characterData = & componentStorage.createComponent(CHARACTER_DATA_TYPE, CharacterData(enemy.getType(), enemy.getID(), false, ENEMY_HEALTH, ENEMY_ATTACK_DAMAGE, ENEMY_ATTACKING_COOLDOWN));
	enemy.node			= & componentStorage.createNode(Sun::SceneNode(device, enemy.physics->position, enemy.physics->rotation, enemy.collider->dim, nullptr, ENEMY_TEXTURE));

	enemy.addComponent(*enemy.physics);
	enemy.addComponent(*enemy.velocity);
	enemy.addComponent(*enemy.collider);
	enemy.addComponent(*enemy.ai);
	enemy.addComponent(*enemy.characterData);

	++enemiesLeft;
}

void EntityManager::createFloor(const char * const tex, const Vector3f& pos, const Vector3f& dim) {
	Entity& floor = entities.emplace_back(FLOOR);

	floor.transformable = & componentStorage.createComponent(TRANSFORMABLE_TYPE, Transformable(floor.getType(), floor.getID(), pos + Vector3f(0, dim.y / 2, 0), Vector3f()));
	floor.node			= & componentStorage.createNode(Sun::SceneNode(device, floor.transformable->position, floor.transformable->rotation, dim, nullptr, tex));

	floor.addComponent(*floor.transformable);
}

void EntityManager::createBullet(const Vector3f& dim) {
	Entity& bullet = entities.emplace_back(BULLET);

	bullet.physics		= & componentStorage.createComponent(PHYSICS_TYPE, Physics(bullet.getType(), bullet.getID(), player->physics->position, Vector3f().getXZfromRotationY(player->physics->rotation.y).normalize() * BULLET_SPEED, player->physics->rotation));
	bullet.bulletData	= & componentStorage.createComponent(BULLET_DATA_TYPE, BulletData(bullet.getType(), bullet.getID(), bullet.physics->velocity.length(), player->characterData->mode, player->characterData->attackDamage));
	bullet.node			= & componentStorage.createNode(Sun::SceneNode(device, bullet.physics->position, bullet.physics->rotation, dim, nullptr, nullptr));

	bullet.node->get()->setTexture(player->characterData->mode ? ANGEL_TEXTURE : DEMON_TEXTURE);

	bullet.addComponent(*bullet.physics);
	bullet.addComponent(*bullet.bulletData);
}

void EntityManager::createPairKeyDoor(const Vector3f& keyPos, const Vector3f& keyDim, const Vector3f& doorPos, const Vector3f& doorDim) {
	Entity& door 	= entities.emplace_back(DOOR);

	door.physics	= & componentStorage.createComponent(PHYSICS_TYPE, Physics(door.getType(), door.getID(), doorPos + Vector3f(0, doorDim.y / 2, 0), Vector3f(), Vector3f()));
	door.collider	= & componentStorage.createComponent(SPECIAL_BOUNDING_BOX_TYPE, BoundingBox(door.getType(), door.getID(), doorDim, door.physics->position, door.physics->velocity, false, STATIC));
	door.node		= & componentStorage.createNode(Sun::SceneNode(device, door.physics->position, door.physics->rotation, door.collider->dim, nullptr, DOOR_TEXTURE));

	door.addComponent(*door.physics);
	door.addComponent(*door.collider);

	Entity& key 	= entities.emplace_back(KEY);

	key.physics		= & componentStorage.createComponent(PHYSICS_TYPE, Physics(key.getType(), key.getID(), keyPos + Vector3f(0, keyDim.y / 2, 0), Vector3f(), Vector3f()));
	key.collider	= & componentStorage.createComponent(SPECIAL_BOUNDING_BOX_TYPE, BoundingBox(key.getType(), key.getID(), keyDim, key.physics->position, key.physics->velocity, true, DYNAMIC));
	key.node		= & componentStorage.createNode(Sun::SceneNode(device, key.physics->position, key.physics->rotation, key.collider->dim, nullptr, KEY_TEXTURE));

	key.addComponent(*key.physics);
	key.addComponent(*key.collider);
}

const Entity& EntityManager::getEntityByID(const std::size_t id) const {
	for (auto& ent : entities)
		if (ent.getID() == id)
			return ent;
	std::cerr << "Entity not found!\n";
	exit(-1);
}

const Entity& EntityManager::getEntityByID(const std::size_t id) {
	return const_cast<Entity&>(std::as_const(*this).getEntityByID(id));
}

void EntityManager::createLevel() {
	initData(128, 16, 128);

	createPairPlayerCamera(Vector3f(), Vector3f(6.f), Vector3f(10, 90, -30));

	//------------ Creacion del escenario para las Christmas ------------------------------------------
	createFloor(CONTROLS_TEXTURE,Vector3f(0,0,5), Vector3f(60,0,35)); //Controls
	createFloor(TIPS_TEXTURE,Vector3f(-2,0,-27), Vector3f(45,0,15)); //Tips

	// Doors and keys
/*
	createPairKeyDoor(Vector3f(0,0,60), Vector3f(3),Vector3f(-37,0,90), Vector3f(2,20,10));
	createPairKeyDoor(Vector3f(-70,0,90), Vector3f(3), Vector3f(37,0,0), Vector3f(2,20,10));
	createPairKeyDoor(Vector3f(70,0,0), Vector3f(3), Vector3f(-37,0,0), Vector3f(2,20,10));
	createPairKeyDoor(Vector3f(-70,0,0), Vector3f(3), Vector3f(37,0,190), Vector3f(2,20,10));
	createPairKeyDoor(Vector3f(70,0,190), Vector3f(3), Vector3f(-37,0,190), Vector3f(2,20,10));
	createPairKeyDoor(Vector3f(-70,0,190), Vector3f(3), Vector3f(37,0,90), Vector3f(2,20,10));
	createPairKeyDoor(Vector3f(70,0,90), Vector3f(3), Vector3f(152.5,0,300), Vector3f(45,10,10));
	createPairKeyDoor(Vector3f(158,0,320), Vector3f(3), Vector3f(-180,0,272.5), Vector3f(10,10,45));
*/

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

	//-------REMOVE IN FUTURE--------------
	/*createMapNode(Vector3f(0,0,250), Vector3f(10));
	createMapNode(Vector3f(0,0,50), Vector3f(10));
	createMapNode(Vector3f(0,0,100), Vector3f(10));
	createMapNode(Vector3f(0,0,150), Vector3f(10));
	createMapNode(Vector3f(0,0,200), Vector3f(10));*/
	//-------REMOVE IN FUTURE--------------


	std::vector<Vector3f> patrol_1 = { Vector3f(-160, 0, 270) };
	std::vector<Vector3f> patrol_2 = { Vector3f(   0, 0, 200) };
	std::vector<Vector3f> patrol_3 = { Vector3f(   0, 0, 100) };
	std::vector<Vector3f> patrol_4 = { Vector3f( 120, 0, 270) };
	std::vector<Vector3f> patrol_5 = { Vector3f(-315, 0, 230), Vector3f(-315, 0, 320), Vector3f(-210, 0, 320), Vector3f(-210, 0, 230) };

/*
	createEnemy(patrol_1[0], Vector3f(8), patrol_1);
	createEnemy(patrol_2[0], Vector3f(8), patrol_2);
	createEnemy(patrol_3[0], Vector3f(8), patrol_3);
	createEnemy(patrol_4[0], Vector3f(8), patrol_4);
*/
	createEnemy(patrol_5[0], Vector3f(8), patrol_5);

}

bool EntityManager::checkVictory() {
	return enemiesLeft <= 0;
}


//REMOVE IN FUTURE

void EntityManager::createMapNode(){

	auto& node_1 = graph.emplace_back(MapNode(0, 0));
	auto& node_2 = graph.emplace_back(MapNode(0, 50));
	auto& node_3 = graph.emplace_back(MapNode(0, 100));
	auto& node_4 = graph.emplace_back(MapNode(0, 200));

	node_1.connections.emplace_back(Connection(&node_1, &node_2, 5));
	node_1.connections.emplace_back(Connection(&node_1, &node_3, 10));
	node_1.connections.emplace_back(Connection(&node_1, &node_1, 2));

	node_2.connections.emplace_back(Connection(&node_2, &node_1, 11));
	node_2.connections.emplace_back(Connection(&node_2, &node_3, 10));
	
	node_3.connections.emplace_back(Connection(&node_3, &node_1, 1));

	//node_4.connections.emplace_back(Connection(&node_4, &node_1, 2));


	std::cout << "	NOOOOOOODOOOOOOOOOS\n";

	for(auto& node : graph)
	{
		std::cout << "Nodo: (" << node.coord.x << ", " << node.coord.y << ")\n";
		for(auto& conn : node.connections)
		{
			std::cout << "Conn to: (" << conn.nodeTo->coord.x << ", " << conn.nodeTo->coord.y << ") / Peso: " << conn.cost << "\n";
		}
		std::cout << "\n";
	}
/*
	std::cout << "	NOOOOOOODOOOOOOOOOS   ORDENADOS\n";

	for(auto& node : graph)
	{
		node.sortConnections();
		std::cout << "Nodo: (" << node.coord.x << ", " << node.coord.y << ")\n";
		for(auto& conn : node.connections)
		{
			std::cout << "Conn to: (" << conn.nodeTo->coord.x << ", " << conn.nodeTo->coord.y << ") / Peso: " << conn.cost << "\n";
		}
		std::cout << "\n";
	}
*/
}

/////////