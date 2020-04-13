#include <man/EntityManager.hpp>

#include <algorithm>

#include <util/ComponentConstants.hpp>
#include <util/TexturePaths.hpp>

#include <Engine/util/Math.hpp>
#include <src/Engine/EngineInterface/SceneInterface/IObjectNode.hpp>
#include <src/Engine/EngineInterface/SceneInterface/ICameraNode.hpp>

void EntityManager::initData(const int maxEntities, const int maxToDelete, const int maxComponents) {
	cleanData();								// si no es la primera vez que llamamos a esta funcion, hay que limpiar vectores, reiniciar variables...
	toDelete.reserve(maxToDelete);				// reservamos para la cantidad maxima de entidades que pueden morir en una sola iteracion del juego
	componentStorage.initData(maxComponents);	// reservamos (de momento la misma) memoria para los vectores que tendran los componentes
	graph.reserve(20);
}


/*		DESTROY ENTITIES	*/

// aqui recibimos los IDS de las entidades que queremos destruir
void EntityManager::addToDestroy(const EntityID ID) {
	const auto it = entities.find(ID);

	if (it != entities.end())
	{
		// obtenemos la entidad que vamos a eliminar
		Entity& e = it->second;

		// si es un enemigo actualizamos la cantidad de enemigos que nos quedan
		// por eliminar para ganar la partida
		if (e.getType() == ENEMY)
			enemiesLeft--;

		// obtenemos su componente de render
		const auto * ren = e.getComponent<Render>();

		// comprobamos que lo tenga (no todas las entidades lo van a tener)
		if (ren) {
			// importante, tenemos que decirle a component storage que ese nodo ahora es libre
			// de ser sobreescrito por otro que necesite alojarse en el vector de nodos
			componentStorage.removeNode(ren->node);
		}

		// destruimos la entidad (anulamos todos sus componentes para que otros puedan ocupar su lugar
		// en el vector de componentes correspondiente)
		e.destroy();

		// borramos la entidad de nuestro contenedor de entidades
		entities.erase(ID);
	}
}

void EntityManager::cleanData() {
	Entity::resetIDManagementValue();		// reiniciamos ID

	for (auto & e : entities)
		if (e.second.getComponent<Render>())
			e.second.getComponent<Render>()->node->remove();

	entities.clear();					    // limpiamos entidades
	toDelete.clear();						// limpiamos vector de entidades a borrar

	graph.clear();                          // limpiamos grafo
	paths.clear();                          // limpiamos caminos

	enemiesLeft = 0;						// reiniciamos valor

	player = nullptr;						// reiniciamos valor
	camera = nullptr;						// reiniciamos valor

	componentStorage.cleanData();			// limpiamos el contenido de component storage
}



/*		CREATE ENTITIES		*/

Entity & EntityManager::createEntity(const EntityType type) {
	auto nextEntityID = Entity::getNextID();

	entities.emplace(nextEntityID, type);

	return entities.find(nextEntityID)->second;
}

void EntityManager::createPairPlayerCamera(const vec3& pos, const vec3& dim, const vec3& posCamera) {
	player = & createEntity(PLAYER);

	auto& velocity 	= componentStorage.createComponent(Velocity(player->getType(), player->getID(), PLAYER_SPEED, PLAYER_ACCELERATION));
	auto& physics  	= componentStorage.createComponent(Physics(player->getType(), player->getID(), pos + vec3(0, dim.y / 2, 0), vec3(), vec3(), dim));
	auto& trigger  	= componentStorage.createComponent(TriggerMovSphere(player->getType(), player->getID(), physics.position, 4, physics.velocity));
	auto& data     	= componentStorage.createComponent(CharacterData(player->getType(), player->getID(), DEMON, PLAYER_HEALTH, PLAYER_SWITCH_MODE_COOLDOWN, PLAYER_ATTACK_DAMAGE, PLAYER_ATTACKING_COOLDOWN, MELEE_ATTACK_RANGE2, PLAYER_DASH_SPEED, PLAYER_DASH_COOLDOWN));
	auto& render	= componentStorage.createComponent(Render(player->getType(), player->getID(), &physics.position, &physics.rotation, &physics.scale, true));

	render.node = componentStorage.createMesh("resources/models/Arc/Arcangel-ZY.obj");

	render.node->setPosition(physics.position);
	render.node->setRotation(physics.rotation);
	render.node->setScale(physics.scale / 2.f);

	render.node->setTexture(DEMON_TEXTURE);

	player->addComponent(velocity);
	player->addComponent(physics);
	player->addComponent(trigger);
	player->addComponent(data);
	player->addComponent(render);


	camera = & createEntity(CAMERA);

	auto& cameraPhysics = componentStorage.createComponent(Physics(camera->getType(), camera->getID(), posCamera, physics.velocity, vec3()));
	auto& cameraRender	= componentStorage.createComponent(Render(camera->getType(), camera->getID(), &cameraPhysics.position, &cameraPhysics.rotation, &cameraPhysics.scale, true));

	cameraRender.node   = componentStorage.createCamera();

	cameraRender.node->setPosition(cameraPhysics.position);
	cameraRender.node->setRotation(cameraPhysics.rotation);
	cameraRender.node->setScale(cameraPhysics.scale);
	cameraRender.node->setTarget(physics.position);

	camera->addComponent(cameraPhysics);
	camera->addComponent(cameraRender);
}

void EntityManager::createLight(const vec3& pos, const vec3& amb, const vec3& diff, const vec3& spe)
{
	auto* light = & createEntity(LIGHT);

	auto& phy     = componentStorage.createComponent(Physics(light->getType(), light->getID(), pos, vec3(), vec3()));
	auto& render  = componentStorage.createComponent(Render(light->getType(), light->getID(), &phy.position, &phy.rotation, &phy.scale, false));

	render.node   = componentStorage.createLight(amb, diff, spe);
	
	render.node->setPosition(phy.position);
	render.node->setPosition(phy.rotation);
	render.node->setPosition(phy.scale);

	light->addComponent(phy);
	light->addComponent(render);

}

void EntityManager::createWall(const vec3& pos, const vec3& dim) {
	auto& wall = createEntity(WALL);

	auto& transformable     = componentStorage.createComponent(Transformable(wall.getType(), wall.getID(), pos + vec3(0, dim.y / 2, 0), vec3(), dim));
	auto& rigidStaticAABB   = componentStorage.createComponent(RigidStaticAABB(wall.getType(), wall.getID(), transformable.position, transformable.scale));
	auto& render			= componentStorage.createComponent(Render(wall.getType(), wall.getID(), &transformable.position, &transformable.rotation, &transformable.scale, false));

	render.node = componentStorage.createMesh("resources/models/Cubo/cuboPrueba.fbx");

	render.node->setPosition(transformable.position);
	render.node->setRotation(transformable.rotation);
	render.node->setScale(transformable.scale / 2.f);

	render.node->setTexture(WALL_TEXTURE);

	wall.addComponent(transformable);
	wall.addComponent(rigidStaticAABB);
	wall.addComponent(render);
}

void EntityManager::createEnemy(const vec3& pos, const vec3& dim, const std::vector<vec3>& patrol, unsigned phase) {
	auto& enemy     = createEntity(ENEMY);

	auto& physics   = componentStorage.createComponent(Physics(enemy.getType(), enemy.getID(), pos + vec3(0, dim.y / 2, 0), vec3(), vec3(), dim));
	auto& velocity  = componentStorage.createComponent(Velocity(enemy.getType(), enemy.getID(), ENEMY_SPEED, ENEMY_ACCELERATION));
	auto& trigger   = componentStorage.createComponent(TriggerMovSphere(enemy.getType(), enemy.getID(), physics.position, 5, physics.velocity));
	auto& data      = componentStorage.createComponent(CharacterData(enemy.getType(), enemy.getID(), NEUTRAL, ENEMY_HEALTH, ENEMY_SWITCH_MODE_COOLDOWN, ENEMY_ATTACK_DAMAGE, ENEMY_ATTACKING_COOLDOWN, MELEE_ATTACK_RANGE2, ENEMY_DASH_SPEED, ENEMY_DASH_COOLDOWN));
	auto& ai        = componentStorage.createComponent(AI(enemy.getType(), enemy.getID(), patrol, phase));
	auto& render	= componentStorage.createComponent(Render(enemy.getType(), enemy.getID(), &physics.position, &physics.rotation, &physics.scale, true));

	render.node = componentStorage.createMesh("resources/models/Cubo/cuboPrueba.fbx");

	render.node->setPosition(physics.position);
	render.node->setRotation(physics.rotation);
	render.node->setScale(physics.scale / 2.f);

	render.node->setTexture(ENEMY_TEXTURE);

	enemy.addComponent(physics);
	enemy.addComponent(velocity);
	enemy.addComponent(trigger);
	enemy.addComponent(data);
	enemy.addComponent(ai);
	enemy.addComponent(render);

	++enemiesLeft;
}

void EntityManager::createAngel(const vec3& pos, const vec3& dim, const std::vector<vec3>& patrol, unsigned phase) {
	auto& enemy     = createEntity(ENEMY);

	auto& physics   = componentStorage.createComponent(Physics(enemy.getType(), enemy.getID(), pos + vec3(0, dim.y / 2, 0), vec3(), vec3(), dim));
	auto& velocity  = componentStorage.createComponent(Velocity(enemy.getType(), enemy.getID(), ENEMY_SPEED, ENEMY_ACCELERATION));
	auto& trigger   = componentStorage.createComponent(TriggerMovSphere(enemy.getType(), enemy.getID(), physics.position, 5, physics.velocity));
	auto& data      = componentStorage.createComponent(CharacterData(enemy.getType(), enemy.getID(), ANGEL, ENEMY_HEALTH, ENEMY_SWITCH_MODE_COOLDOWN, ENEMY_ATTACK_DAMAGE, ENEMY_ATTACKING_COOLDOWN, DIST_ATTACK_RANGE2, ENEMY_DASH_SPEED, ENEMY_DASH_COOLDOWN));
	auto& ai        = componentStorage.createComponent(AI(enemy.getType(), enemy.getID(), patrol, phase));
	auto& render	= componentStorage.createComponent(Render(enemy.getType(), enemy.getID(), &physics.position, &physics.rotation, &physics.scale, true));

	render.node = componentStorage.createMesh("resources/models/Cubo/cuboPrueba.fbx");

	render.node->setPosition(physics.position);
	render.node->setRotation(physics.rotation);
	render.node->setScale(physics.scale / 2.f);

	render.node->setTexture(ENEMY_TEXTURE);

	enemy.addComponent(physics);
	enemy.addComponent(velocity);
	enemy.addComponent(trigger);
	enemy.addComponent(data);
	enemy.addComponent(ai);
	enemy.addComponent(render);

	++enemiesLeft;
}

void EntityManager::createDemon(const vec3& pos, const vec3& dim, const std::vector<vec3>& patrol, unsigned phase) {
	auto& enemy     = createEntity(ENEMY);
	auto& physics   = componentStorage.createComponent(Physics(enemy.getType(), enemy.getID(), pos + vec3(0, dim.y / 2, 0), vec3(), vec3(), dim));
	auto& velocity  = componentStorage.createComponent(Velocity(enemy.getType(), enemy.getID(), ENEMY_SPEED, ENEMY_ACCELERATION));
	auto& trigger   = componentStorage.createComponent(TriggerMovSphere(enemy.getType(), enemy.getID(), physics.position, 5, physics.velocity));
	auto& data      = componentStorage.createComponent(CharacterData(enemy.getType(), enemy.getID(), DEMON, ENEMY_HEALTH, ENEMY_SWITCH_MODE_COOLDOWN, ENEMY_ATTACK_DAMAGE, ENEMY_ATTACKING_COOLDOWN, MELEE_ATTACK_RANGE2, ENEMY_DASH_SPEED, ENEMY_DASH_COOLDOWN));
	auto& jump 		= componentStorage.createComponent(Jump(enemy.getType(), enemy.getID(), JUMP_COOLDOWN, JUMP_TIMER));
	auto& ai        = componentStorage.createComponent(AI(enemy.getType(), enemy.getID(), patrol, phase));
	auto& render	= componentStorage.createComponent(Render(enemy.getType(), enemy.getID(), &physics.position, &physics.rotation, &physics.scale, true));

	render.node = componentStorage.createMesh("resources/models/Cubo/cuboPrueba.fbx");

	render.node->setPosition(physics.position);
	render.node->setRotation(physics.rotation);
	render.node->setScale(physics.scale / 2.f);

	render.node->setTexture(ENEMY_TEXTURE);

	enemy.addComponent(physics);
	enemy.addComponent(velocity);
	enemy.addComponent(trigger);
	enemy.addComponent(data);
	enemy.addComponent(jump);
	enemy.addComponent(ai);
	enemy.addComponent(render);

	++enemiesLeft;
}

void EntityManager::createFloor(const std::string_view tex, const vec3& pos, const vec3& dim) {
	auto& floor = createEntity(FLOOR);

	auto& transformable = componentStorage.createComponent(Transformable(floor.getType(), floor.getID(), pos + vec3(0, dim.y / 2, 0), vec3(-90, 0, 0), vec3(1)));
	auto& render		= componentStorage.createComponent(Render(floor.getType(), floor.getID(), &transformable.position, &transformable.rotation, &transformable.scale, false));

	render.node = componentStorage.createMesh("resources/models/Ciudad/ciudad.fbx");

	render.node->setPosition(transformable.position);
	render.node->setRotation(transformable.rotation);
	render.node->setScale(transformable.scale * 2.f);

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
	auto& render	= componentStorage.createComponent(Render(bullet.getType(), bullet.getID(), &physics.position, &physics.rotation, &physics.scale, true));

	render.node = componentStorage.createMesh("resources/models/Cubo/cuboPrueba.fbx");

	render.node->setPosition(physics.position);
	render.node->setRotation(physics.rotation);
	render.node->setScale(physics.scale / 2.f);

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
	auto& render			= componentStorage.createComponent(Render(door.getType(), door.getID(), &transformable.position, &transformable.rotation, &transformable.scale, false));

	render.node = componentStorage.createMesh("resources/models/Cubo/cuboPrueba.fbx");

	render.node->setPosition(transformable.position);
	render.node->setRotation(transformable.rotation);
	render.node->setScale(transformable.scale / 2.f);

	render.node->setTexture(DOOR_TEXTURE);

	door.addComponent(transformable);
    door.addComponent(trigger);
	door.addComponent(rigid);
	door.addComponent(render);

	Entity& key 		    = createEntity(KEY);

	auto& keyTransformable  = componentStorage.createComponent(Transformable(key.getType(), key.getID(), keyPos + vec3(0, keyDim.y / 2, 0), vec3(), keyDim));
	auto& keyTrigger        = componentStorage.createComponent(TriggerStaticAABB(key.getType(), key.getID(), keyTransformable.position, keyTransformable.scale, true));
	auto& keyRender			= componentStorage.createComponent(Render(key.getType(), key.getID(), &keyTransformable.position, &keyTransformable.rotation, &keyTransformable.scale, false));

	keyRender.node = componentStorage.createMesh("resources/models/Cubo/cuboPrueba.fbx");

	keyRender.node->setPosition(keyTransformable.position);
	keyRender.node->setRotation(keyTransformable.rotation);
	keyRender.node->setScale(keyTransformable.scale / 2.f);

	keyRender.node->setTexture(KEY_TEXTURE);

	key.addComponent(keyTransformable);
	key.addComponent(keyTrigger);
	key.addComponent(keyRender);
}

void EntityManager::setNavConnections(const GraphNode & node, const std::vector<const GraphNode *> & conns) const {
	// MAX_GRAPH_CONN especifica cuantas conexiones puede haber como maximo
	// si se necesita añadir mas conexiones, ir a util/ComponentConstants.hpp
	// y cambiar el valor

	// esta linea significa "asegurate de que MAX_GRAPH_CONN es >= al size de
	// conns, de lo contrario aborta la ejecución"
	// assert avisara de lo que esta pasando con un mensaje de error por consola
	assert(MAX_GRAPH_CONN >= conns.size());

	// set devuelve iteradores const, asi que para modificarlos tenemos que usar
	// const_cast (siempre que haya uno tratando GraphNode es por eso)
	// PERO CUIDADO no modificar lo que set utilice para ordenar (position en este caso)
	auto & n = const_cast<GraphNode &>(node);

	// guardamos en cada conexion, la distancia al nodo y un puntero a dicho nodo
	// los valores que no rellenemos quedaran a <INFINITY, nullptr>
	for (n.numConns = 0; n.numConns < conns.size(); ++n.numConns) {
		n.conns[n.numConns] = {
				distance(n.position, conns[n.numConns]->position),
				const_cast<GraphNode *>(conns[n.numConns])
		};
	}

	// ordenamos las conexiones por distancia, que es lo que guarda el primer valor del pair
	std::sort( n.conns, n.conns + n.numConns, ComparePairLessThan() );
}

void EntityManager::createNavigation() {
	nav = & createEntity(NAV);

	auto & nav_graph = componentStorage.createComponent(Graph(nav->getType(), nav->getID()));

	// hall
	GraphNode  gn0 ( {    0,  0 } );
	GraphNode  gn1 ( {    0, 50 } );
	GraphNode  gn2 ( {    0,100 } );
	GraphNode  gn3 ( {    0,150 } );
	GraphNode  gn4 ( {    0,200 } );
	GraphNode  gn5 ( {    0,260 } );

	// right
	GraphNode  gn6 ( {   60,270 } );
	GraphNode  gn7 ( {  130,270 } );

	// left
	GraphNode  gn8 ( {  -80,270 } );
	GraphNode  gn9 ( { -160,270 } );

	// square
	GraphNode gn10 ( { -200,275 } );
	GraphNode gn11 ( { -210,320 } );
	GraphNode gn12 ( { -210,230 } );
	GraphNode gn13 ( { -315,230 } );
	GraphNode gn14 ( { -315,320 } );

	nav_graph.nodes.insert( {
		 gn0,  gn1,  gn2,  gn3,  gn4,
		 gn5,  gn6,  gn7,  gn8,  gn9,
		gn10, gn11, gn12, gn13, gn14
	} );

	auto &  i0 = * nav_graph.nodes.find  (gn0);
	auto &  i1 = * nav_graph.nodes.find  (gn1);
	auto &  i2 = * nav_graph.nodes.find  (gn2);
	auto &  i3 = * nav_graph.nodes.find  (gn3);
	auto &  i4 = * nav_graph.nodes.find  (gn4);
	auto &  i5 = * nav_graph.nodes.find  (gn5);

	auto &  i6 = * nav_graph.nodes.find  (gn6);
	auto &  i7 = * nav_graph.nodes.find  (gn7);

	auto &  i8 = * nav_graph.nodes.find  (gn8);
	auto &  i9 = * nav_graph.nodes.find  (gn9);

	auto & i10 = * nav_graph.nodes.find (gn10);
	auto & i11 = * nav_graph.nodes.find (gn11);
	auto & i12 = * nav_graph.nodes.find (gn12);
	auto & i13 = * nav_graph.nodes.find (gn13);
	auto & i14 = * nav_graph.nodes.find (gn14);

	setNavConnections(  i0, {             &i1 } );
	setNavConnections(  i1, {       &i0,  &i2 } );
	setNavConnections(  i2, {       &i1,  &i3 } );
	setNavConnections(  i3, {       &i2,  &i4 } );
	setNavConnections(  i4, {       &i3,  &i5 } );
	setNavConnections(  i5, { &i4,  &i6,  &i8 } );
	setNavConnections(  i6, {       &i5,  &i7 } );
	setNavConnections(  i7, {             &i6 } );
	setNavConnections(  i8, {       &i5,  &i9 } );
	setNavConnections(  i9, {       &i8, &i10 } );
	setNavConnections( i10, { &i9, &i11, &i12 } );
	setNavConnections( i11, {      &i10, &i14 } );
	setNavConnections( i12, {      &i10, &i13 } );
	setNavConnections( i13, {      &i12, &i14 } );
	setNavConnections( i14, {      &i11, &i13 } );

	nav->addComponent<Graph>(nav_graph);
}

const Entity& EntityManager::getEntityByID(const EntityID id) const {
	return entities.find(id)->second;
}

Entity& EntityManager::getEntityByID(const EntityID id) {
	return const_cast<Entity &>(std::as_const(*this).getEntityByID(id));
}

void EntityManager::createLevel() {
	initData(128, 16, 150);
	
	createPairPlayerCamera(vec3(), vec3(6.f), vec3(30, 120, 70));
	createLight(vec3(30, 60, 20), vec3(0.1), vec3(0.6), vec3(0.2));

	//createFloor(CONTROLS_TEXTURE, vec3(0,0,-5), vec3(60,0,35)); //Controls

	//------------ Creacion del escenario para las Christmas ------------------------------------------
	// Doors and keys
	createPairKeyDoor(vec3(0,0,-60), vec3(3), vec3(-37,0,-90), vec3(2,20,10));
	createPairKeyDoor(vec3(-70,0,-90), vec3(3), vec3(37,0,0), vec3(2,20,10));
	createPairKeyDoor(vec3(70,0,0), vec3(3), vec3(-37,0,0), vec3(2,20,10));
	createPairKeyDoor(vec3(-70,0,0), vec3(3), vec3(37,0,-190), vec3(2,20,10));
	createPairKeyDoor(vec3(70,0,-190), vec3(3), vec3(-37,0,-190), vec3(2,20,10));
	createPairKeyDoor(vec3(-70,0,-190), vec3(3), vec3(37,0,-90), vec3(2,20,10));
	createPairKeyDoor(vec3(70,0,-90), vec3(3), vec3(152.5,0,-300), vec3(45,10,10));
	createPairKeyDoor(vec3(158,0,-320), vec3(3), vec3(-180,0,-272.5), vec3(10,10,45));

	//Pasillo inicial

	//Derecha
	createWall(vec3(40,0,27.5), vec3(10,10,45));
	createWall(vec3(40,0,-45), vec3(10,10,80));
	createWall(vec3(40,0,-140), vec3(10,10,90));
	createWall(vec3(40,0,-222.5), vec3(10,10,55));

	//Izquierda
	createWall(vec3(-40,0,27.5), vec3(10,10,45));
	createWall(vec3(-40,0,-45), vec3(10,10,80));
	createWall(vec3(-40,0,-140), vec3(10,10,90));
	createWall(vec3(-40,0,-222.5), vec3(10,10,55));

	createWall(vec3(0,0,45), vec3(70,10,10));      //Cierre inferior

	//Salas del pasillo

	//Sala 1
	createWall(vec3(75,0,30), vec3(60,10,10));    //Abajo
	createWall(vec3(75,0,-30), vec3(60,10,10));    //Arriba
	createWall(vec3(100,0,0), vec3(10,10,50));    //Cierre

	//Sala 2
	createWall(vec3(-75,0,30), vec3(60,10,10));    //Abajo
	createWall(vec3(-75,0,-30), vec3(60,10,10));    //Arriba
	createWall(vec3(-100,0,0), vec3(10,10,50));    //Cierre

	//Sala 3
	createWall(vec3(75,0,-60), vec3(60,10,10));    //Abajo
	createWall(vec3(75,0,-120), vec3(60,10,10));    //Arriba
	createWall(vec3(100,0,-90), vec3(10,10,50));    //Cierre

	//Sala 4
	createWall(vec3(-75,0,-60), vec3(60,10,10));    //Abajo
	createWall(vec3(-75,0,-120), vec3(60,10,10));    //Arriba
	createWall(vec3(-100,0,-90), vec3(10,10,50));    //Cierre

	//Sala 5
	createWall(vec3(-75,0,-160), vec3(60,10,10));    //Abajo
	createWall(vec3(-75,0,-220), vec3(60,10,10));    //Arriba
	createWall(vec3(-100,0,-190), vec3(10,10,50));    //Cierre

	//Sala 6
	createWall(vec3(75,0,-160), vec3(60,10,10));    //Abajo
	createWall(vec3(75,0,-220), vec3(60,10,10));    //Arriba
	createWall(vec3(100,0,-190), vec3(10,10,50));    //Cierre

	//Pasillo Horizontal
	createWall(vec3(-115,0,-245), vec3(140,10,10));    //Inferior izda
	createWall(vec3(115,0,-245), vec3(140,10,10));     //Inderior dcha
	createWall(vec3(-27.5,0,-300), vec3(315,10,10));   //Superior
	createWall(vec3(180,0,-277.5), vec3(10,10,55));  //Derecha

	//Sala llave principal
	createWall(vec3(200,0,-300), vec3(30,10,10));
	createWall(vec3(210,0,-345), vec3(10,10,80));
	createWall(vec3(100,0,-345), vec3(10,10,80));
	createWall(vec3(157.5,0,-380), vec3(105,10,10));

	//Zona patrulla
	createWall(vec3(-180,0,-227.5), vec3(10,10,45)); //Inf der
	createWall(vec3(-180,0,-327.5), vec3(10,10,45)); //Sup der
	createWall(vec3(-262.5,0,-345), vec3(160,10,10)); //Sup
	createWall(vec3(-262.5,0,-210), vec3(160,10,10)); //Inf
	createWall(vec3(-347.5,0,-277.5), vec3(10,10,145)); //Izq
	createWall(vec3(-265,0,-277.5), vec3(60,20,55)); //Pilar
	//------------------------------------  END MAPA  ---------------------------------------------------------------

	createGraph();
	//puntero a vec3? Mejor?
	std::vector<vec3> patrol_1 = { graph[9].coord };
	std::vector<vec3> patrol_2 = { graph[4].coord };
	std::vector<vec3> patrol_3 = { graph[5].coord };
	std::vector<vec3> patrol_4 = { graph[6].coord };
	std::vector<vec3> patrol_5 = { graph[13].coord, graph[14].coord, graph[11].coord, graph[12].coord };
	std::vector<vec3> patrol_6 = { graph[13].coord, graph[14].coord };
	std::vector<vec3> patrol_7 = { graph[11].coord, graph[12].coord };
	std::vector<vec3> patrol_8 = { graph[12].coord, graph[13].coord };
	std::vector<vec3> patrol_9 = { graph[14].coord, graph[11].coord };
	std::vector<vec3> patrol_10 = { graph[7].coord };
	std::vector<vec3> patrol_11 = { graph[8].coord };
	std::vector<vec3> patrol_12 = { graph[8].coord };

	unsigned scheduling_AI_counter = 0;
//
	createDemon(patrol_2[0], vec3(11), patrol_2, ++scheduling_AI_counter);
	createEnemy(patrol_1[0], vec3(8), patrol_1, ++scheduling_AI_counter);
	createEnemy(patrol_3[0], vec3(8), patrol_3, ++scheduling_AI_counter);
	createEnemy(patrol_4[0], vec3(8), patrol_4, ++scheduling_AI_counter);
	createEnemy(patrol_5[0], vec3(8), patrol_5, ++scheduling_AI_counter);
	createEnemy(patrol_6[0], vec3(8), patrol_6, ++scheduling_AI_counter);
	createEnemy(patrol_7[0], vec3(8), patrol_7, ++scheduling_AI_counter);
	createEnemy(patrol_8[0], vec3(8), patrol_8, ++scheduling_AI_counter);
	createEnemy(patrol_9[0], vec3(8), patrol_9, ++scheduling_AI_counter);
	createEnemy(patrol_10[0], vec3(8), patrol_10, ++scheduling_AI_counter);
	createEnemy(patrol_11[0], vec3(8), patrol_11, ++scheduling_AI_counter);
	createEnemy(patrol_12[0], vec3(8), patrol_12, ++scheduling_AI_counter);
//
	createDemon(patrol_2[0], vec3(11), patrol_2, ++scheduling_AI_counter);
	createEnemy(patrol_1[0], vec3(8), patrol_1, ++scheduling_AI_counter);
	createEnemy(patrol_3[0], vec3(8), patrol_3, ++scheduling_AI_counter);
	createEnemy(patrol_4[0], vec3(8), patrol_4, ++scheduling_AI_counter);
	createEnemy(patrol_5[0], vec3(8), patrol_5, ++scheduling_AI_counter);
	createEnemy(patrol_6[0], vec3(8), patrol_6, ++scheduling_AI_counter);
	createEnemy(patrol_7[0], vec3(8), patrol_7, ++scheduling_AI_counter);
	createEnemy(patrol_8[0], vec3(8), patrol_8, ++scheduling_AI_counter);
	createEnemy(patrol_9[0], vec3(8), patrol_9, ++scheduling_AI_counter);
	createEnemy(patrol_10[0], vec3(8), patrol_10, ++scheduling_AI_counter);
	createEnemy(patrol_11[0], vec3(8), patrol_11, ++scheduling_AI_counter);
	createEnemy(patrol_12[0], vec3(8), patrol_12, ++scheduling_AI_counter);

	createDemon(patrol_2[0], vec3(11), patrol_2, ++scheduling_AI_counter);
	createEnemy(patrol_1[0], vec3(8), patrol_1, ++scheduling_AI_counter);
	createEnemy(patrol_3[0], vec3(8), patrol_3, ++scheduling_AI_counter);
	createEnemy(patrol_4[0], vec3(8), patrol_4, ++scheduling_AI_counter);
	createEnemy(patrol_5[0], vec3(8), patrol_5, ++scheduling_AI_counter);
	createEnemy(patrol_6[0], vec3(8), patrol_6, ++scheduling_AI_counter);
	createEnemy(patrol_7[0], vec3(8), patrol_7, ++scheduling_AI_counter);
	createEnemy(patrol_8[0], vec3(8), patrol_8, ++scheduling_AI_counter);
	createEnemy(patrol_9[0], vec3(8), patrol_9, ++scheduling_AI_counter);
	createEnemy(patrol_10[0], vec3(8), patrol_10, ++scheduling_AI_counter);
	createEnemy(patrol_11[0], vec3(8), patrol_11, ++scheduling_AI_counter);
	createEnemy(patrol_12[0], vec3(8), patrol_12, ++scheduling_AI_counter);
	
	createDemon(patrol_2[0], vec3(11), patrol_2, ++scheduling_AI_counter);
	createEnemy(patrol_1[0], vec3(8), patrol_1, ++scheduling_AI_counter);
	createEnemy(patrol_3[0], vec3(8), patrol_3, ++scheduling_AI_counter);
	createEnemy(patrol_4[0], vec3(8), patrol_4, ++scheduling_AI_counter);
	createEnemy(patrol_5[0], vec3(8), patrol_5, ++scheduling_AI_counter);
	createEnemy(patrol_6[0], vec3(8), patrol_6, ++scheduling_AI_counter);
	createEnemy(patrol_7[0], vec3(8), patrol_7, ++scheduling_AI_counter);
	createEnemy(patrol_8[0], vec3(8), patrol_8, ++scheduling_AI_counter);
	createEnemy(patrol_9[0], vec3(8), patrol_9, ++scheduling_AI_counter);
	createEnemy(patrol_10[0], vec3(8), patrol_10, ++scheduling_AI_counter);
	createEnemy(patrol_11[0], vec3(8), patrol_11, ++scheduling_AI_counter);
	createEnemy(patrol_12[0], vec3(8), patrol_12, ++scheduling_AI_counter);

	createDemon(patrol_2[0], vec3(11), patrol_2, ++scheduling_AI_counter);
	createEnemy(patrol_1[0], vec3(8), patrol_1, ++scheduling_AI_counter);
	createEnemy(patrol_3[0], vec3(8), patrol_3, ++scheduling_AI_counter);
	createEnemy(patrol_4[0], vec3(8), patrol_4, ++scheduling_AI_counter);
	createEnemy(patrol_5[0], vec3(8), patrol_5, ++scheduling_AI_counter);
	createEnemy(patrol_6[0], vec3(8), patrol_6, ++scheduling_AI_counter);
	createEnemy(patrol_7[0], vec3(8), patrol_7, ++scheduling_AI_counter);
	createEnemy(patrol_8[0], vec3(8), patrol_8, ++scheduling_AI_counter);
	createEnemy(patrol_9[0], vec3(8), patrol_9, ++scheduling_AI_counter);
	createEnemy(patrol_10[0], vec3(8), patrol_10, ++scheduling_AI_counter);
	createEnemy(patrol_11[0], vec3(8), patrol_11, ++scheduling_AI_counter);
	createEnemy(patrol_12[0], vec3(8), patrol_12, ++scheduling_AI_counter);
	
	createDemon(patrol_2[0], vec3(11), patrol_2, ++scheduling_AI_counter);
	createEnemy(patrol_1[0], vec3(8), patrol_1, ++scheduling_AI_counter);
	createEnemy(patrol_3[0], vec3(8), patrol_3, ++scheduling_AI_counter);
	createEnemy(patrol_4[0], vec3(8), patrol_4, ++scheduling_AI_counter);
	createEnemy(patrol_5[0], vec3(8), patrol_5, ++scheduling_AI_counter);
	createEnemy(patrol_6[0], vec3(8), patrol_6, ++scheduling_AI_counter);
	createEnemy(patrol_7[0], vec3(8), patrol_7, ++scheduling_AI_counter);
	createEnemy(patrol_8[0], vec3(8), patrol_8, ++scheduling_AI_counter);
	createEnemy(patrol_9[0], vec3(8), patrol_9, ++scheduling_AI_counter);
	createEnemy(patrol_10[0], vec3(8), patrol_10, ++scheduling_AI_counter);
	createEnemy(patrol_11[0], vec3(8), patrol_11, ++scheduling_AI_counter);
	createEnemy(patrol_12[0], vec3(8), patrol_12, ++scheduling_AI_counter);

	createDemon(patrol_2[0], vec3(11), patrol_2, ++scheduling_AI_counter);
	createEnemy(patrol_1[0], vec3(8), patrol_1, ++scheduling_AI_counter);
	createEnemy(patrol_3[0], vec3(8), patrol_3, ++scheduling_AI_counter);
	createEnemy(patrol_4[0], vec3(8), patrol_4, ++scheduling_AI_counter);
	createEnemy(patrol_5[0], vec3(8), patrol_5, ++scheduling_AI_counter);
	createEnemy(patrol_6[0], vec3(8), patrol_6, ++scheduling_AI_counter);
	createEnemy(patrol_7[0], vec3(8), patrol_7, ++scheduling_AI_counter);
	createEnemy(patrol_8[0], vec3(8), patrol_8, ++scheduling_AI_counter);
	createEnemy(patrol_9[0], vec3(8), patrol_9, ++scheduling_AI_counter);
	createEnemy(patrol_10[0], vec3(8), patrol_10, ++scheduling_AI_counter);
	createEnemy(patrol_11[0], vec3(8), patrol_11, ++scheduling_AI_counter);

	std::cout << componentStorage.getComponents<Transformable>()[0].getName()     << " " << componentStorage.getComponents<Transformable>().size() << "\n";
	std::cout << componentStorage.getComponents<Physics>()[0].getName()           << " " << componentStorage.getComponents<Physics>().size() << "\n";
	std::cout << componentStorage.getComponents<RigidStaticAABB>()[0].getName()   << " " << componentStorage.getComponents<RigidStaticAABB>().size() << "\n";
	std::cout << componentStorage.getComponents<TriggerMovSphere>()[0].getName()  << " " << componentStorage.getComponents<TriggerMovSphere>().size() << "\n";
	std::cout << componentStorage.getComponents<AI>()[0].getName()                << " " << componentStorage.getComponents<AI>().size() << "\n";
	std::cout << componentStorage.getComponents<TriggerStaticAABB>()[0].getName() << " " << componentStorage.getComponents<TriggerStaticAABB>().size() << "\n";
	std::cout << componentStorage.getComponents<Render>()[0].getName()            << " " << componentStorage.getComponents<Render>().size() << "\n";
	std::cout << componentStorage.getComponents<Velocity>()[0].getName()          << " " << componentStorage.getComponents<Velocity>().size() << "\n";
	std::cout << componentStorage.getComponents<CharacterData>()[0].getName()     << " " << componentStorage.getComponents<CharacterData>().size() << "\n";
}

void EntityManager::createGraph()
{
	//hall
	auto& node_0  = graph.emplace_back(MapNode(0, 0));
	auto& node_1  = graph.emplace_back(MapNode(0, -50));
	auto& node_2  = graph.emplace_back(MapNode(0, -100));
	auto& node_3  = graph.emplace_back(MapNode(0, -150));
	auto& node_4  = graph.emplace_back(MapNode(0, -200));
	auto& node_5  = graph.emplace_back(MapNode(0, -260));
	//right
	auto& node_6  = graph.emplace_back(MapNode(60, -270));
	auto& node_7  = graph.emplace_back(MapNode(130, -270));
	//left
	auto& node_8  = graph.emplace_back(MapNode(-80, -270));
	auto& node_9  = graph.emplace_back(MapNode(-160, -270));
	//square
	auto& node_10 = graph.emplace_back(MapNode(-200, -275));
	auto& node_11 = graph.emplace_back(MapNode(-210, -320));
	auto& node_12 = graph.emplace_back(MapNode(-210, -230));
	auto& node_13 = graph.emplace_back(MapNode(-315, -230));
	auto& node_14 = graph.emplace_back(MapNode(-315, -320));

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

bool EntityManager::checkVictory() const {
	return enemiesLeft <= 0;
}

bool EntityManager::checkDefeat() const {
	return !greater_e(player->getComponent<CharacterData>()->health, 0.f);
}
