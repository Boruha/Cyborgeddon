#include <man/EntityManager.hpp>

#include <algorithm>

#include <util/ComponentConstants.hpp>
#include <util/TexturePaths.hpp>

#include <Engine/util/Math.hpp>
#include <src/Engine/EngineInterface/SceneInterface/IObjectNode.hpp>
#include <src/Engine/EngineInterface/SceneInterface/ICameraNode.hpp>

/*		Init - Update	*/
void EntityManager::init() {
	createLevel();	// de momento se crea asi sin mas, pero habra que utilizar un fichero de texto para generar el nivel
}

void EntityManager::initData(const int maxEntities, const int maxToDelete, const int maxComponents) {
	cleanData();								// si no es la primera vez que llamamos a esta funcion, hay que limpiar vectores, reiniciar variables...
	entities.reserve(maxEntities);				// reservamos memoria para la cantidad maxima de entidades esperada
	toDelete.reserve(maxToDelete);				// lo mismo para la cantidad maxima de entidades que pueden morir en una sola iteracion del juego
	componentStorage.initData(maxComponents);	// reservamos (de momento la misma) memoria para los vectores que tendran los componentes
	graph.reserve(20);
}

bool EntityManager::update(){

	if (!toDelete.empty())			// si hay entidades que "matar"
		killEntities();				// las matamos

	if (entitiesLeftToDelete > 0) {
		moveDeadEntities();				// las entidades muertas se van moviendo hacia el final
		removeEntities();				// las entidades que estan al final son eliminadas del vector
	}

	return checkVictory();
}

bool EntityManager::checkVictory() {
	if (!greater_e(player->characterData->health, 0)) {
		std::cout << "\n\nFin de partida\n\n";
		exit(-1);
	}

	return enemiesLeft <= 0;
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
			continue;									// si la entidad que queremos eliminar tiene nodo
		if (e->inode) {
			e->inode->get()->remove();
			e->inode = nullptr;
		}

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

void EntityManager::createPairPlayerCamera(const vec3& pos, const vec3& dim, const vec3& posCamera) {
	player = & entities.emplace_back(PLAYER);
	camera = & entities.emplace_back(CAMERA);

	player->velocity        = & componentStorage.createComponent<Velocity>(VELOCITY_TYPE, player->getType(), player->getID(), PLAYER_SPEED, PLAYER_ACCELERATION);
	player->physics         = & componentStorage.createComponent<Physics>(PHYSICS_TYPE, player->getType(), player->getID(), pos + vec3(0, dim.y / 2, 0), vec3(), vec3());
	player->collider		= & componentStorage.createComponent<BoundingBox>(SPECIAL_BOUNDING_BOX_TYPE, player->getType(), player->getID(), dim, player->physics->position, player->physics->velocity, true, DYNAMIC);
	player->characterData   = & componentStorage.createComponent<CharacterData>(CHARACTER_DATA_TYPE, player->getType(), player->getID(), DEMON, PLAYER_HEALTH, PLAYER_SWITCH_MODE_COOLDOWN, PLAYER_ATTACK_DAMAGE, PLAYER_ATTACKING_COOLDOWN, PLAYER_DASH_SPEED, PLAYER_DASH_COOLDOWN);
	player->inode			= &componentStorage.createIObjectNode<IObjectNode>(&player->physics->position, &player->physics->rotation, &player->collider->dim);
	player->inode->get()->setTexture(DEMON_TEXTURE);

	camera->physics			= & componentStorage.createComponent<Physics>(PHYSICS_TYPE, camera->getType(), camera->getID(), posCamera, player->physics->velocity, vec3());
	camera->inode			= & componentStorage.createICameraNode<ICameraNode>(&camera->physics->position, &camera->physics->rotation, &camera->physics->scale, &player->physics->position);
}

void EntityManager::createWall(const vec3& pos, const vec3& dim) {
	Entity& wall 		= entities.emplace_back(WALL);

	wall.transformable	= & componentStorage.createComponent<Transformable>(TRANSFORMABLE_TYPE, wall.getType(), wall.getID(), pos + vec3(0, dim.y / 2, 0), vec3());
	wall.collider		= & componentStorage.createComponent<BoundingBox>(STATIC_BOUNDING_BOX_TYPE, wall.getType(), wall.getID(), dim, wall.transformable->position, false, STATIC);
	wall.inode			= & componentStorage.createIObjectNode<IObjectNode>(&wall.transformable->position, &wall.transformable->rotation, &wall.collider->dim);
	wall.inode->get()->setTexture(WALL_TEXTURE);
}

void EntityManager::createEnemy(const vec3& pos, const vec3& dim, const std::vector<vec3>& patrol) {
	Entity& enemy 		= entities.emplace_back(ENEMY);

	enemy.physics		= & componentStorage.createComponent<Physics>(PHYSICS_TYPE, enemy.getType(), enemy.getID(), pos + vec3(0, dim.y / 2, 0), vec3(), vec3());
	enemy.velocity		= & componentStorage.createComponent<Velocity>(VELOCITY_TYPE, enemy.getType(), enemy.getID(), ENEMY_SPEED, ENEMY_ACCELERATION);
	enemy.collider		= & componentStorage.createComponent<BoundingBox>(SPECIAL_BOUNDING_BOX_TYPE, enemy.getType(), enemy.getID(), dim, enemy.physics->position, enemy.physics->velocity, false, STATIC);
	enemy.ai			= & componentStorage.createComponent<AI>(AI_TYPE, enemy.getType(), enemy.getID(), patrol);
	enemy.characterData = & componentStorage.createComponent<CharacterData>(CHARACTER_DATA_TYPE, enemy.getType(), enemy.getID(), NEUTRAL, ENEMY_HEALTH, ENEMY_SWITCH_MODE_COOLDOWN, ENEMY_ATTACK_DAMAGE, ENEMY_ATTACKING_COOLDOWN, ENEMY_DASH_SPEED, ENEMY_DASH_COOLDOWN);
	enemy.inode			= & componentStorage.createIObjectNode<IObjectNode>(&enemy.physics->position, &enemy.physics->rotation, &enemy.collider->dim);
	enemy.inode->get()->setTexture(ENEMY_TEXTURE);

	++enemiesLeft;
}

void EntityManager::createFloor(const char * const tex, const vec3& pos, const vec3& dim) {
	Entity& floor 		= entities.emplace_back(FLOOR);

	floor.transformable = & componentStorage.createComponent<Transformable>(TRANSFORMABLE_TYPE, floor.getType(), floor.getID(), pos + vec3(0, dim.y / 2, 0), vec3(), dim);
	floor.inode			= & componentStorage.createIObjectNode<IObjectNode>(&floor.transformable->position, &floor.transformable->rotation, &floor.transformable->scale);
	floor.inode->get()->setTexture(tex);
}

void EntityManager::createBullet() {
	Entity& bullet 		= entities.emplace_back(BULLET);

	bullet.physics		= & componentStorage.createComponent<Physics>(PHYSICS_TYPE, bullet.getType(), bullet.getID(), player->physics->position, normalize(getXZfromRotationY(player->physics->rotation.y)) * BULLET_SPEED, player->physics->rotation, vec3(0.5, 0, player->collider->dim.z));
	bullet.bulletData	= & componentStorage.createComponent<BulletData>(BULLET_DATA_TYPE, bullet.getType(), bullet.getID(), length(bullet.physics->velocity), player->characterData->mode, player->characterData->attackDamage);
	bullet.inode		= & componentStorage.createIObjectNode<IObjectNode>(&bullet.physics->position, &bullet.physics->rotation, &bullet.physics->scale);
	bullet.inode->get()->setTexture(player->characterData->mode ? ANGEL_TEXTURE : DEMON_TEXTURE);
}

void EntityManager::createPairKeyDoor(const vec3& keyPos, const vec3& keyDim, const vec3& doorPos, const vec3& doorDim) {
	Entity& door 		= entities.emplace_back(DOOR);

	door.transformable 	= & componentStorage.createComponent<Transformable>(TRANSFORMABLE_TYPE, door.getType(), door.getID(), doorPos + vec3(0, doorDim.y / 2, 0), vec3(), doorDim);
	door.collider		= & componentStorage.createComponent<BoundingBox>(SPECIAL_BOUNDING_BOX_TYPE, door.getType(), door.getID(), door.transformable->scale, door.transformable->position, false, STATIC);
	door.inode			= & componentStorage.createIObjectNode<IObjectNode>(&door.transformable->position, &door.transformable->rotation, &door.transformable->scale);
	door.inode->get()->setTexture(DOOR_TEXTURE);

	Entity& key 		= entities.emplace_back(KEY);

	key.transformable	= & componentStorage.createComponent<Transformable>(TRANSFORMABLE_TYPE, key.getType(), key.getID(), keyPos + vec3(0, keyDim.y / 2, 0), vec3(), keyDim);
	key.collider		= & componentStorage.createComponent<BoundingBox>(SPECIAL_BOUNDING_BOX_TYPE, key.getType(), key.getID(), key.transformable->scale, key.transformable->position, true, DYNAMIC);
	key.inode			= & componentStorage.createIObjectNode<IObjectNode>(&key.transformable->position, &key.transformable->rotation, &key.transformable->scale);
	key.inode->get()->setTexture(KEY_TEXTURE);
}

const Entity& EntityManager::getEntityByID(const std::size_t id) const {
	for (const auto& ent : entities)
		if (ent.getID() == id)
			return ent;
	std::cerr << "Entity not found?!?!\n";
	exit(-1);
}

Entity& EntityManager::getEntityByID(const std::size_t id) {
	return const_cast<Entity&>(std::as_const(*this).getEntityByID(id));
}

void EntityManager::createLevel() {
	initData(128, 16, 128);

	createPairPlayerCamera(vec3(), vec3(6.f), vec3(10, 120, -90));

	//------------ Creacion del escenario para las Christmas ------------------------------------------
	createFloor(CONTROLS_TEXTURE, vec3(0,0,5), vec3(60,0,35)); //Controls
	createFloor(TIPS_TEXTURE, vec3(-2,0,-27), vec3(45,0,15)); //Tips

	// Doors and keys
	createPairKeyDoor(vec3(0,0,60), vec3(3), vec3(-37,0,90), vec3(2,20,10));
	createPairKeyDoor(vec3(-70,0,90), vec3(3), vec3(37,0,0), vec3(2,20,10));
	createPairKeyDoor(vec3(70,0,0), vec3(3), vec3(-37,0,0), vec3(2,20,10));
	createPairKeyDoor(vec3(-70,0,0), vec3(3), vec3(37,0,190), vec3(2,20,10));
	createPairKeyDoor(vec3(70,0,190), vec3(3), vec3(-37,0,190), vec3(2,20,10));
	createPairKeyDoor(vec3(-70,0,190), vec3(3), vec3(37,0,90), vec3(2,20,10));
	createPairKeyDoor(vec3(70,0,90), vec3(3), vec3(152.5,0,300), vec3(45,10,10));
	createPairKeyDoor(vec3(158,0,320), vec3(3), vec3(-180,0,272.5), vec3(10,10,45));

	//Pasillo inicial

	//Derecha
	createWall(vec3(40,0,-27.5), vec3(10,10,45));
	createWall(vec3(40,0,45), vec3(10,10,80));
	createWall(vec3(40,0,140), vec3(10,10,90));
	createWall(vec3(40,0,222.5), vec3(10,10,55));

	//Izquierda
	createWall(vec3(-40,0,-27.5), vec3(10,10,45));
	createWall(vec3(-40,0,45), vec3(10,10,80));
	createWall(vec3(-40,0,140), vec3(10,10,90));
	createWall(vec3(-40,0,222.5), vec3(10,10,55));

	createWall(vec3(0,0,-45), vec3(70,10,10));      //Cierre inferior

	//Salas del pasillo

	//Sala 1
	createWall(vec3(75,0,-30), vec3(60,10,10));    //Abajo
	createWall(vec3(75,0,30), vec3(60,10,10));    //Arriba
	createWall(vec3(100,0,0), vec3(10,10,50));    //Cierre

	//Sala 2
	createWall(vec3(-75,0,-30), vec3(60,10,10));    //Abajo
	createWall(vec3(-75,0,30), vec3(60,10,10));    //Arriba
	createWall(vec3(-100,0,0), vec3(10,10,50));    //Cierre

	//Sala 3
	createWall(vec3(75,0,60), vec3(60,10,10));    //Abajo
	createWall(vec3(75,0,120), vec3(60,10,10));    //Arriba
	createWall(vec3(100,0,90), vec3(10,10,50));    //Cierre

	//Sala 4
	createWall(vec3(-75,0,60), vec3(60,10,10));    //Abajo
	createWall(vec3(-75,0,120), vec3(60,10,10));    //Arriba
	createWall(vec3(-100,0,90), vec3(10,10,50));    //Cierre

	//Sala 5
	createWall(vec3(-75,0,160), vec3(60,10,10));    //Abajo
	createWall(vec3(-75,0,220), vec3(60,10,10));    //Arriba
	createWall(vec3(-100,0,190), vec3(10,10,50));    //Cierre

	//Sala 6
	createWall(vec3(75,0,160), vec3(60,10,10));    //Abajo
	createWall(vec3(75,0,220), vec3(60,10,10));    //Arriba
	createWall(vec3(100,0,190), vec3(10,10,50));    //Cierre

	//Pasillo Horizontal
	createWall(vec3(-115,0,245), vec3(140,10,10));    //Inferior izda
	createWall(vec3(115,0,245), vec3(140,10,10));     //Inderior dcha
	createWall(vec3(-27.5,0,300), vec3(315,10,10));   //Superior
	createWall(vec3(180,0,277.5), vec3(10,10,55));  //Derecha

	//Sala llave principal
	createWall(vec3(200,0,300), vec3(30,10,10));
	createWall(vec3(210,0,345), vec3(10,10,80));
	createWall(vec3(100,0,345), vec3(10,10,80));
	createWall(vec3(157.5,0,380), vec3(105,10,10));

	//Zona patrulla
	createWall(vec3(-180,0,227.5), vec3(10,10,45)); //Inf der
	createWall(vec3(-180,0,327.5), vec3(10,10,45)); //Sup der
	createWall(vec3(-262.5,0,345), vec3(160,10,10)); //Sup
	createWall(vec3(-262.5,0,210), vec3(160,10,10)); //Inf
	createWall(vec3(-347.5,0,277.5), vec3(10,10,145)); //Izq
	createWall(vec3(-265,0,277.5), vec3(60,20,55)); //Pilar
	//------------------------------------  END MAPA  ---------------------------------------------------------------

	createGraph();
	//puntero a vec3? Mejor?
	std::vector<vec3> patrol_1 = { graph[9].coord };
	std::vector<vec3> patrol_2 = { graph[4].coord };
	std::vector<vec3> patrol_3 = { graph[5].coord };
	std::vector<vec3> patrol_4 = { graph[7].coord };
	std::vector<vec3> patrol_5 = { graph[13].coord, graph[14].coord, graph[11].coord, graph[12].coord };

	createEnemy(patrol_1[0], vec3(8), patrol_1);
	createEnemy(patrol_2[0], vec3(8), patrol_2);
	createEnemy(patrol_3[0], vec3(8), patrol_3);
	createEnemy(patrol_4[0], vec3(8), patrol_4);
	createEnemy(patrol_5[0], vec3(8), patrol_5);
}

void EntityManager::createGraph()
{
	//hall
	auto& node_0  = graph.emplace_back(MapNode(0, 0));
	auto& node_1  = graph.emplace_back(MapNode(0, 50));
	auto& node_2  = graph.emplace_back(MapNode(0, 100));
	auto& node_3  = graph.emplace_back(MapNode(0, 150));
	auto& node_4  = graph.emplace_back(MapNode(0, 200));
	auto& node_5  = graph.emplace_back(MapNode(0, 260));
	//right
	auto& node_6  = graph.emplace_back(MapNode(60, 270));
	auto& node_7  = graph.emplace_back(MapNode(130, 270));
	//left
	auto& node_8  = graph.emplace_back(MapNode(-80, 270));
	auto& node_9  = graph.emplace_back(MapNode(-160, 270));
	//square
	auto& node_10 = graph.emplace_back(MapNode(-200, 275));
	auto& node_11 = graph.emplace_back(MapNode(-210, 320));
	auto& node_12 = graph.emplace_back(MapNode(-210, 230));
	auto& node_13 = graph.emplace_back(MapNode(-315, 230));
	auto& node_14 = graph.emplace_back(MapNode(-315, 320));

	node_0.connections.emplace_back(0, 1, 5);

	node_1.connections.emplace_back(1, 0, 5);
	node_1.connections.emplace_back(1, 2, 5);
	
	node_2.connections.emplace_back(2, 1, 5);
	node_2.connections.emplace_back(2, 3, 5);

	node_3.connections.emplace_back(3, 2, 5);
	node_3.connections.emplace_back(3, 4, 5);

	node_4.connections.emplace_back(4, 3, 5);
	node_4.connections.emplace_back(4, 5, 5);


	node_5.connections.emplace_back(5, 4, 5);
	node_5.connections.emplace_back(5, 6, 7);
	node_5.connections.emplace_back(5, 8, 7);

	node_6.connections.emplace_back(6, 5, 7);
	node_6.connections.emplace_back(6, 7, 6);

	node_7.connections.emplace_back(7, 6, 6);

	node_8.connections.emplace_back(8, 5, 7);
	node_8.connections.emplace_back(8, 9, 6);

	node_9.connections.emplace_back(9, 8, 6);
	node_9.connections.emplace_back(9, 10, 8);

	node_10.connections.emplace_back(10, 9, 8);
	node_10.connections.emplace_back(10, 11, 6);
	node_10.connections.emplace_back(10, 12, 6);

	node_11.connections.emplace_back(11, 10, 6);
	node_11.connections.emplace_back(11, 14, 9);

	node_12.connections.emplace_back(12, 10, 6);
	node_12.connections.emplace_back(12, 13, 9);

	node_13.connections.emplace_back(13, 12, 9);
	node_13.connections.emplace_back(13, 14, 9);

	node_14.connections.emplace_back(14, 11, 9);
	node_14.connections.emplace_back(14, 13, 9);

}