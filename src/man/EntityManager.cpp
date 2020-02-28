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
	toDelete.reserve(maxToDelete);				// reservamos para la cantidad maxima de entidades que pueden morir en una sola iteracion del juego
	componentStorage.initData(maxComponents);	// reservamos (de momento la misma) memoria para los vectores que tendran los componentes
	graph.reserve(20);
}

bool EntityManager::update(){

	if (!toDelete.empty())			// si hay entidades que "matar"
		killEntities();				// las matamos

	return checkVictory();
}

bool EntityManager::checkVictory() {
	if (!greater_e(player->getComponent<CharacterData>()->health, 0)) {
		std::cout << "\n\nFin de partida\n\n";
		exit(-1);
	}

	return enemiesLeft <= 0;
}

/*		DESTROY ENTITIES	*/

// aqui recibimos los IDS de las entidades que queremos destruir
void EntityManager::addToDestroy(EntityID ID) {
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

	for (const auto & d : toDelete) {
		map_entities.find(d)->second.destroy();
		map_entities.erase(d);
	}

	std::cout << map_entities.size() << std::endl;

	toDelete.clear();							// al acabar de matar las entidades, limpipamos el vector toDelete
}

void EntityManager::cleanData() {
	Entity::resetIDManagementValue();		// reiniciamos ID

	map_entities.clear();					// limpiamos entidades
	toDelete.clear();						// limpiamos vector de entidades a borrar

	entitiesLeftToDelete = 0;				// reiniciamos valor
	enemiesLeft = 0;						// reiniciamos valor

	player = nullptr;						// reiniciamos valor
	camera = nullptr;						// reiniciamos valor

	componentStorage.cleanData();			// limpiamos el contenido de component storage
}

/*		CREATE ENTITIES		*/



Entity & EntityManager::createEntity(const EntityType type) {
	auto nextEntityID = Entity::getNextID();

	map_entities.emplace(nextEntityID, type);

	return map_entities.find(nextEntityID)->second;
}


void EntityManager::createPairPlayerCamera(const vec3& pos, const vec3& dim, const vec3& posCamera) {
	player = & createEntity(PLAYER);

	auto& velocity 	= componentStorage.createComponent(Velocity(player->getType(), player->getID(), PLAYER_SPEED, PLAYER_ACCELERATION));
	auto& physics  	= componentStorage.createComponent(Physics(player->getType(), player->getID(), pos + vec3(0, dim.y / 2, 0), vec3(), vec3(), dim));
	auto& trigger  	= componentStorage.createComponent(TriggerMovSphere(player->getType(), player->getID(), physics.position, 4, physics.velocity));
	auto& data     	= componentStorage.createComponent(CharacterData(player->getType(), player->getID(), DEMON, PLAYER_HEALTH, PLAYER_SWITCH_MODE_COOLDOWN, PLAYER_ATTACK_DAMAGE, PLAYER_ATTACKING_COOLDOWN, PLAYER_DASH_SPEED, PLAYER_DASH_COOLDOWN));
	auto& render	= componentStorage.createComponent(Render(player->getType(), player->getID(), &physics.position, &physics.rotation, &physics.scale));

	render.node = componentStorage.createIObjectNode();

	render.node->setPosition(pos);
	render.node->setRotation(vec3(0));
	render.node->setScale(dim);

	render.node->setTexture(DEMON_TEXTURE);

	player->addComponent(velocity);
	player->addComponent(physics);
	player->addComponent(trigger);
	player->addComponent(data);
	player->addComponent(render);



	camera = & createEntity(CAMERA);

	auto& cameraPhysics = componentStorage.createComponent(Physics(camera->getType(), camera->getID(), posCamera, physics.velocity, vec3()));
	auto& cameraRender	= componentStorage.createComponent(Render(camera->getType(), camera->getID(), &cameraPhysics.position, &cameraPhysics.rotation, &cameraPhysics.scale));

	cameraRender.node = componentStorage.createICameraNode();

	cameraRender.node->setPosition(posCamera);
	cameraRender.node->setRotation(vec3(0));
	cameraRender.node->setScale(vec3(1));
	cameraRender.node->setTarget(pos);

	camera->addComponent(cameraPhysics);
	camera->addComponent(cameraRender);
}

void EntityManager::createWall(const vec3& pos, const vec3& dim) {
	auto& wall = createEntity(WALL);

	auto& transformable     = componentStorage.createComponent(Transformable(wall.getType(), wall.getID(), pos + vec3(0, dim.y / 2, 0), vec3(), dim));
	auto& rigidStaticAABB   = componentStorage.createComponent(RigidStaticAABB(wall.getType(), wall.getID(), transformable.position, transformable.scale));
	auto& render			= componentStorage.createComponent(Render(wall.getType(), wall.getID(), &transformable.position, &transformable.rotation, &transformable.scale));

	render.node = componentStorage.createIObjectNode();

	render.node->setPosition(pos);
	render.node->setRotation(vec3(0));
	render.node->setScale(dim);

	render.node->setTexture(WALL_TEXTURE);

	wall.addComponent(transformable);
	wall.addComponent(rigidStaticAABB);
	wall.addComponent(render);
}

void EntityManager::createEnemy(const vec3& pos, const vec3& dim, const std::vector<vec3>& patrol) {
	auto& enemy     = createEntity(ENEMY);

	auto& physics   = componentStorage.createComponent(Physics(enemy.getType(), enemy.getID(), pos + vec3(0, dim.y / 2, 0), vec3(), vec3(), dim));
	auto& velocity  = componentStorage.createComponent(Velocity(enemy.getType(), enemy.getID(), ENEMY_SPEED, ENEMY_ACCELERATION));
	auto& trigger   = componentStorage.createComponent(TriggerMovSphere(enemy.getType(), enemy.getID(), physics.position, 5, physics.velocity));
	auto& data      = componentStorage.createComponent(CharacterData(enemy.getType(), enemy.getID(), NEUTRAL, ENEMY_HEALTH, ENEMY_SWITCH_MODE_COOLDOWN, ENEMY_ATTACK_DAMAGE, ENEMY_ATTACKING_COOLDOWN, ENEMY_DASH_SPEED, ENEMY_DASH_COOLDOWN));
	auto& ai        = componentStorage.createComponent(AI(enemy.getType(), enemy.getID(), patrol));
	auto& render	= componentStorage.createComponent(Render(enemy.getType(), enemy.getID(), &physics.position, &physics.rotation, &physics.scale));

	render.node = componentStorage.createIObjectNode();

	render.node->setPosition(pos);
	render.node->setRotation(vec3(0));
	render.node->setScale(dim);

	render.node->setTexture(ENEMY_TEXTURE);

	enemy.addComponent(physics);
	enemy.addComponent(velocity);
	enemy.addComponent(trigger);
	enemy.addComponent(data);
	enemy.addComponent(ai);
	enemy.addComponent(render);

	++enemiesLeft;
}

void EntityManager::createFloor(const std::string_view tex, const vec3& pos, const vec3& dim) {
	auto& floor = createEntity(FLOOR);

	auto& transformable = componentStorage.createComponent(Transformable(floor.getType(), floor.getID(), pos + vec3(0, dim.y / 2, 0), vec3(), dim));
	auto& render		= componentStorage.createComponent(Render(floor.getType(), floor.getID(), &transformable.position, &transformable.rotation, &transformable.scale));

	render.node = componentStorage.createIObjectNode();

	render.node->setPosition(pos);
	render.node->setRotation(vec3(0));
	render.node->setScale(dim);

	render.node->setTexture(tex.data());

	floor.addComponent(transformable);
	floor.addComponent(render);
}

void EntityManager::createBullet() {
	auto& bullet = createEntity(BULLET);

	auto * playerPhysics = player->getComponent<Physics>();
	auto * playerTrigger = player->getComponent<TriggerMovSphere>();
	auto * playerData    = player->getComponent<CharacterData>();

	auto& physics   = componentStorage.createComponent(Physics(bullet.getType(), bullet.getID(), playerPhysics->position, normalize(getXZfromRotationY(playerPhysics->rotation.y)) * BULLET_SPEED, playerPhysics->rotation, vec3(0.5, 0, playerTrigger->radius)));
	auto& data      = componentStorage.createComponent(BulletData(bullet.getType(), bullet.getID(), length(physics.velocity), playerData->mode, playerData->attackDamage));
	auto& trigger   = componentStorage.createComponent(TriggerFastMov(bullet.getType(), bullet.getID(), physics.position, physics.velocity));
	auto& render	= componentStorage.createComponent(Render(bullet.getType(), bullet.getID(), &physics.position, &physics.rotation, &physics.scale));

	render.node = componentStorage.createIObjectNode();

	render.node->setPosition(physics.position);
	render.node->setRotation(physics.rotation);
	render.node->setScale(physics.scale);

	render.node->setTexture(playerData->mode ? ANGEL_TEXTURE : DEMON_TEXTURE);

	bullet.addComponent(physics);
	bullet.addComponent(data);
	bullet.addComponent(trigger);
	bullet.addComponent(render);
}

void EntityManager::createPairKeyDoor(const vec3& keyPos, const vec3& keyDim, const vec3& doorPos, const vec3& doorDim) {
	Entity& door 		    = createEntity(DOOR);

	auto& transformable     = componentStorage.createComponent(Transformable(door.getType(), door.getID(), doorPos + vec3(0, doorDim.y / 2, 0), vec3(), doorDim));
	auto& trigger           = componentStorage.createComponent(TriggerStaticAABB(door.getType(), door.getID(), transformable.position, transformable.scale, false));
	auto& rigid             = componentStorage.createComponent(RigidStaticAABB(door.getType(), door.getID(), transformable.position, transformable.scale));
	auto& render			= componentStorage.createComponent(Render(door.getType(), door.getID(), &transformable.position, &transformable.rotation, &transformable.scale));

	render.node = componentStorage.createIObjectNode();

	render.node->setPosition(transformable.position);
	render.node->setRotation(transformable.rotation);
	render.node->setScale(transformable.scale);

	render.node->setTexture(DOOR_TEXTURE);

	door.addComponent(transformable);
    door.addComponent(trigger);
	door.addComponent(rigid);
	door.addComponent(render);

	Entity& key 		    = createEntity(KEY);

	auto& keyTransformable  = componentStorage.createComponent(Transformable(key.getType(), key.getID(), keyPos + vec3(0, keyDim.y / 2, 0), vec3(), keyDim));
	auto& keyTrigger        = componentStorage.createComponent(TriggerStaticAABB(key.getType(), key.getID(), keyTransformable.position, keyTransformable.scale, true));
	auto& keyRender			= componentStorage.createComponent(Render(key.getType(), key.getID(), &keyTransformable.position, &keyTransformable.rotation, &keyTransformable.scale));

	keyRender.node = componentStorage.createIObjectNode();

	keyRender.node->setPosition(keyTransformable.position);
	keyRender.node->setRotation(keyTransformable.rotation);
	keyRender.node->setScale(keyTransformable.scale);

	keyRender.node->setTexture(KEY_TEXTURE);

	key.addComponent(keyTransformable);
	key.addComponent(keyTrigger);
	key.addComponent(keyRender);
}

const Entity& EntityManager::getEntityByID(const EntityID id) const {
	return map_entities.find(id)->second;
}

Entity& EntityManager::getEntityByID(const EntityID id) {
	return const_cast<Entity &>(std::as_const(*this).getEntityByID(id));
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