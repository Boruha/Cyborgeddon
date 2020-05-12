#include <man/EntityManager.hpp>

#include <algorithm>

#include <util/ComponentConstants.hpp>
#include <util/TexturePaths.hpp>
#include <util/HUDPaths.hpp>

#include <Engine/util/Math.hpp>
#include <src/Engine/EngineInterface/SceneInterface/IObjectNode.hpp>
#include <src/Engine/EngineInterface/SceneInterface/ICameraNode.hpp>

#include <Engine/EngineInterface/SceneInterface/IVideo.hpp>
#include <Engine/EngineInterface/SceneInterface/ITexture.hpp>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

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

	idxVideo = 0;                           // reiniciamos valor

	player = nullptr;						// reiniciamos valor
	camera = nullptr;						// reiniciamos valor

	componentStorage.cleanData();			// limpiamos el contenido de component storage

	engine.unloadVideos();                  // limpiamos videos del motor si habia alguno en memoria
	engine.unloadTextures();
}

void EntityManager::processMesh(aiMesh * mesh) {
	if (mesh->mNumVertices != 3) {
		std::cerr << "En el obj hay un objeto que no es un triangulo\n";
	} else {
		std::array<vec3, 3> vertices {};

		for (unsigned i = 0; i < 3; ++i)
			vertices[i] = glm::vec3(mesh->mVertices[i].x, 0, mesh->mVertices[i].z);

		auto & collider = createEntity(COLLIDER);

		auto & obb   = componentStorage.createComponent(TriangleOBB(collider.getType(), collider.getID(), vertices));

		collider.addComponent(obb);
	}
}

void EntityManager::processNode(aiNode * node, const aiScene * scene) {
	for (unsigned i = 0; i < node->mNumMeshes; ++i)
		processMesh(scene->mMeshes[node->mMeshes[i]]);
	for (unsigned i = 0; i < node->mNumChildren; ++i)
		processNode(node->mChildren[i], scene);
}

void EntityManager::readColliderFile(const std::string_view path) {
	Assimp::Importer importer;

	const aiScene * scene = importer.ReadFile(path.data(), 0);

	if (scene)
		processNode(scene->mRootNode, scene);
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
	auto& physics  	= componentStorage.createComponent(Physics(player->getType(), player->getID(), pos, vec3(), vec3(), dim));
	auto& trigger  	= componentStorage.createComponent(TriggerMovSphere(player->getType(), player->getID(), physics.position, 0.5f, physics.velocity));
	auto& circleBounding = componentStorage.createComponent(CircleBounding(player->getType(), player->getID(), physics.position, 0.5f, physics.velocity));
	auto& data     	= componentStorage.createComponent(CharacterData(player->getType(), player->getID(), DEMON, PLAYER_HEALTH, PLAYER_SWITCH_MODE_COOLDOWN, PLAYER_ATTACK_DAMAGE, PLAYER_ATTACKING_COOLDOWN, MELEE_ATTACK_RANGE2, PLAYER_DASH_SPEED, PLAYER_DASH_COOLDOWN));
	auto& render	= componentStorage.createComponent(Render(player->getType(), player->getID(), &physics.position, &physics.rotation, &physics.scale, true));

	render.node = componentStorage.createMesh("resources/models/Dex/Static/0.obj");


	render.node->setPosition(physics.position);
	render.node->setRotation(physics.rotation);
	render.node->setScale(physics.scale);

	render.node->setTexture(DEX_DEMONIC_TEXTURE);

	player->addComponent(velocity);
	player->addComponent(physics);
	player->addComponent(trigger);
	player->addComponent(circleBounding);
	player->addComponent(data);
	player->addComponent(render);


	camera = & createEntity(CAMERA);

	auto& cameraPhysics = componentStorage.createComponent(Physics(camera->getType(), camera->getID(), posCamera, physics.velocity, vec3(0)));
	auto& cameraRender	= componentStorage.createComponent(Render(camera->getType(), camera->getID(), &cameraPhysics.position, &cameraPhysics.rotation, &cameraPhysics.scale, true));

	cameraRender.node   = componentStorage.createCamera();

	cameraRender.node->setPosition(cameraPhysics.position);
	cameraRender.node->setRotation(cameraPhysics.rotation);
	cameraRender.node->setScale(cameraPhysics.scale);
	cameraRender.node->setTarget(physics.position);

	camera->addComponent(cameraPhysics);
	camera->addComponent(cameraRender);
}

void EntityManager::createLight(const vec3& pos, const vec3& dir, const vec3& diff, const vec3& spe){
	light = & createEntity(LIGHT);

	auto& phy     = componentStorage.createComponent(Physics(light->getType(), light->getID(), pos, vec3(0), vec3(0)));
	auto& render  = componentStorage.createComponent(Render(light->getType(), light->getID(), &phy.position, &phy.rotation, &phy.scale, true));

	render.node   = componentStorage.createLight(diff, spe, dir);
	render.node->setPosition(phy.position);

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

	auto& physics   = componentStorage.createComponent(Physics(enemy.getType(), enemy.getID(), pos, vec3(), vec3(), dim));
	auto& velocity  = componentStorage.createComponent(Velocity(enemy.getType(), enemy.getID(), ENEMY_SPEED, ENEMY_ACCELERATION));
	auto& trigger   = componentStorage.createComponent(TriggerMovSphere(enemy.getType(), enemy.getID(), physics.position, 1, physics.velocity));
	auto& circleBounding = componentStorage.createComponent(CircleBounding(enemy.getType(), enemy.getID(), physics.position, 1.f, physics.velocity));
	auto& data      = componentStorage.createComponent(CharacterData(enemy.getType(), enemy.getID(), NEUTRAL, ENEMY_HEALTH, ENEMY_SWITCH_MODE_COOLDOWN, ENEMY_ATTACK_DAMAGE, ENEMY_ATTACKING_COOLDOWN, MELEE_ATTACK_RANGE2, ENEMY_DASH_SPEED, ENEMY_DASH_COOLDOWN));
	auto& ai        = componentStorage.createComponent(AI(enemy.getType(), enemy.getID(), patrol, phase));
	auto& render	= componentStorage.createComponent(Render(enemy.getType(), enemy.getID(), &physics.position, &physics.rotation, &physics.scale, true));

	render.node = componentStorage.createMesh("resources/models/Bot/Static/0.obj");


	render.node->setPosition(physics.position);
	render.node->setRotation(physics.rotation);
	render.node->setScale(physics.scale);

	render.node->setTexture(ENEMY_TEXTURE);

	enemy.addComponent(physics);
	enemy.addComponent(velocity);
	enemy.addComponent(trigger);
	enemy.addComponent(circleBounding);
	enemy.addComponent(data);
	enemy.addComponent(ai);
	enemy.addComponent(render);

	++enemiesLeft;
}

void EntityManager::createAngel(const vec3& pos, const vec3& dim, const std::vector<vec3>& patrol, unsigned phase) {
	auto& enemy     = createEntity(ENEMY);

	auto& physics   = componentStorage.createComponent(Physics(enemy.getType(), enemy.getID(), pos, vec3(), vec3(), dim));
	auto& velocity  = componentStorage.createComponent(Velocity(enemy.getType(), enemy.getID(), ENEMY_SPEED, ENEMY_ACCELERATION));
	auto& trigger   = componentStorage.createComponent(TriggerMovSphere(enemy.getType(), enemy.getID(), physics.position, 5, physics.velocity));
	auto& circleBounding = componentStorage.createComponent(CircleBounding(enemy.getType(), enemy.getID(), physics.position, 5.f, physics.velocity));
	auto& data      = componentStorage.createComponent(CharacterData(enemy.getType(), enemy.getID(), ANGEL, ENEMY_HEALTH, ENEMY_SWITCH_MODE_COOLDOWN, ENEMY_ATTACK_DAMAGE, ENEMY_ATTACKING_COOLDOWN, DIST_ATTACK_RANGE2, ENEMY_DASH_SPEED, ENEMY_DASH_COOLDOWN));
	auto& ai        = componentStorage.createComponent(AI(enemy.getType(), enemy.getID(), patrol, phase));
	auto& render	= componentStorage.createComponent(Render(enemy.getType(), enemy.getID(), &physics.position, &physics.rotation, &physics.scale, true));

	render.node = componentStorage.createMesh("resources/models/Angel/Static/0.obj");

	render.node->setPosition(physics.position);
	render.node->setRotation(physics.rotation);
	render.node->setScale(physics.scale);

	render.node->setTexture(ANGEL_TEXTURE);

	enemy.addComponent(physics);
	enemy.addComponent(velocity);
	enemy.addComponent(trigger);
	enemy.addComponent(circleBounding);
	enemy.addComponent(data);
	enemy.addComponent(ai);
	enemy.addComponent(render);

	++enemiesLeft;
}

void EntityManager::createDemon(const vec3& pos, const vec3& dim, const std::vector<vec3>& patrol, unsigned phase) {
	auto& enemy     = createEntity(ENEMY);
	auto& physics   = componentStorage.createComponent(Physics(enemy.getType(), enemy.getID(), pos, vec3(), vec3(), dim));
	auto& velocity  = componentStorage.createComponent(Velocity(enemy.getType(), enemy.getID(), ENEMY_SPEED, ENEMY_ACCELERATION));
	auto& trigger   = componentStorage.createComponent(TriggerMovSphere(enemy.getType(), enemy.getID(), physics.position, 5, physics.velocity));
	auto& circleBounding = componentStorage.createComponent(CircleBounding(enemy.getType(), enemy.getID(), physics.position, 5.f, physics.velocity));
	auto& data      = componentStorage.createComponent(CharacterData(enemy.getType(), enemy.getID(), DEMON, ENEMY_HEALTH, ENEMY_SWITCH_MODE_COOLDOWN, ENEMY_ATTACK_DAMAGE, ENEMY_ATTACKING_COOLDOWN, MELEE_ATTACK_RANGE2, ENEMY_DASH_SPEED, ENEMY_DASH_COOLDOWN));
	auto& jump 		= componentStorage.createComponent(Jump(enemy.getType(), enemy.getID(), JUMP_COOLDOWN, JUMP_TIMER));
	auto& ai        = componentStorage.createComponent(AI(enemy.getType(), enemy.getID(), patrol, phase));
	auto& render	= componentStorage.createComponent(Render(enemy.getType(), enemy.getID(), &physics.position, &physics.rotation, &physics.scale, true));

	render.node = componentStorage.createMesh("resources/models/Demon/Static/0.obj");


	render.node->setPosition(physics.position);
	render.node->setRotation(physics.rotation);
	render.node->setScale(physics.scale);

	render.node->setTexture(DEMON_TEXTURE);

	enemy.addComponent(physics);
	enemy.addComponent(velocity);
	enemy.addComponent(trigger);
	enemy.addComponent(circleBounding);
	enemy.addComponent(data);
	enemy.addComponent(jump);
	enemy.addComponent(ai);
	enemy.addComponent(render);

	++enemiesLeft;
}

void EntityManager::createFloor(const std::string_view tex, const vec3& pos, const vec3& dim) {
	auto& floor = createEntity(FLOOR);

	auto& transformable = componentStorage.createComponent(Transformable(floor.getType(), floor.getID(), pos + vec3(0, 0, 0), vec3(0, 0, 0), vec3(1)));
	auto& render		= componentStorage.createComponent(Render(floor.getType(), floor.getID(), &transformable.position, &transformable.rotation, &transformable.scale, false));

	render.node = componentStorage.createMesh("resources/models/Ciudad/ciudad.obj");

	render.node->setPosition(transformable.position);
	render.node->setRotation(transformable.rotation);
	render.node->setScale(transformable.scale);

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
	//TODO: quitar la mierda de  "+ vec3 (0, y/2, 0) en transformable mas adelante"
	auto& transformable     = componentStorage.createComponent(Transformable(door.getType(), door.getID(), doorPos + vec3(0, 0, 0), vec3(), doorDim));
	auto& trigger           = componentStorage.createComponent(TriggerStaticAABB(door.getType(), door.getID(), transformable.position, transformable.scale, false));
	auto& rigid             = componentStorage.createComponent(RigidStaticAABB(door.getType(), door.getID(), transformable.position, transformable.scale));
	auto& render			= componentStorage.createComponent(Render(door.getType(), door.getID(), &transformable.position, &transformable.rotation, &transformable.scale, false));

	render.node = componentStorage.createMesh("resources/models/Objetos/Puerta/0.obj");

	render.node->setPosition(transformable.position);
	render.node->setRotation(transformable.rotation);
	render.node->setScale(transformable.scale);

	render.node->setTexture(DOOR_TEXTURE);

	door.addComponent(transformable);
    door.addComponent(trigger);
	door.addComponent(rigid);
	door.addComponent(render);

	Entity& key 		    = createEntity(KEY);

	auto& keyTransformable  = componentStorage.createComponent(Transformable(key.getType(), key.getID(), keyPos + vec3(0, 0, 0), vec3(), keyDim));
	auto& keyTrigger        = componentStorage.createComponent(TriggerStaticAABB(key.getType(), key.getID(), keyTransformable.position, keyTransformable.scale, true));
	auto& keyRender			= componentStorage.createComponent(Render(key.getType(), key.getID(), &keyTransformable.position, &keyTransformable.rotation, &keyTransformable.scale, false));

	keyRender.node = componentStorage.createMesh("resources/models/Objetos/Llave_demoniaca/0.obj");

	keyRender.node->setPosition(keyTransformable.position);
	keyRender.node->setRotation(keyTransformable.rotation);
	keyRender.node->setScale(keyTransformable.scale);

	keyRender.node->setTexture(DEMONIC_KEY_TEXTURE);

	key.addComponent(keyTransformable);
	key.addComponent(keyTrigger);
	key.addComponent(keyRender);
}


void EntityManager::createVideo(const std::string_view path, const bool isLoop) {
	auto& video = createEntity(VIDEO);

	auto& cmpVideo = componentStorage.createComponent(Video(video.getType(), video.getID(), path));

	cmpVideo.video = engine.loadVideo(path);

	cmpVideo.frameCounter = 0;
	cmpVideo.numFrames = unsigned(cmpVideo.video->getNumFrames());
	cmpVideo.timePerFrame = float(1.f / cmpVideo.video->getFPS());

	cmpVideo.loop = isLoop;

	cmpVideo.video->setLoop(cmpVideo.loop);

	video.addComponent(cmpVideo);
}

void EntityManager::createTexture(const std::string_view path, const int x, const int y) {
	auto& texture = createEntity(TEXTURE);

	auto& cmpTexture = componentStorage.createComponent(TextureCmp(texture.getType(), texture.getID()));

	cmpTexture.texture = engine.loadTexture(path);

	cmpTexture.texture->setPosition(x,y);

	texture.addComponent(cmpTexture);
}

void EntityManager::createMenuOptions(const unsigned int firstOption, const unsigned int maxOptions) {
	auto& menu_options = createEntity(MENU_OPTIONS);

	auto& options = componentStorage.createComponent(MenuOption(menu_options.getType(), menu_options.getID(), int(firstOption), maxOptions));

	menu_options.addComponent(options);
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
	cleanData();

	initData(128, 16, 150);
	
	createPairPlayerCamera(vec3(), vec3(1.f), vec3(5, 50, 10)); //x= , y= , z=    <------> antes: vec3(30, 120, 70) - (10, 50, 10)
	createLight(vec3(-40, 80, -60), vec3(-11.0, -22.0, 8.0), vec3(0.8), vec3(0.0));
	//createLight(vec3(0, 30, 20), vec3(1.0, -1.0, -1.0), vec3(0.6), vec3(0.5)); 
	//createLight(vec3(20,20, 20), vec3(-1.0, -1.0, -1.0), vec3(0.6), vec3(0.2));

	createFloor(CONTROLS_TEXTURE, vec3(0,0,0), vec3(0,0,0)); //Controls
	readColliderFile("resources/models/Ciudad/colisiones.obj");

	createTexture(KEY_2, 558, 52);
	createTexture(KEY_1, 517, 52);
	createTexture(MAIN_HUD, 0, 35);
	createTexture(ANGEL_LIFEBAR, 0, 86);
	createTexture(DEMON_LIFEBAR, 0, 86);
	createTexture(BACKGROUND_LIFEBAR, 0, 78);

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

	//CREACION DE LAS LLAVES
    //createPairKeyDoor(vec3(0,0,-20), vec3(1), vec3(0,0,-10), vec3(1));


	//DISTRIBUCION DE ENEMIGOS -> (x=derecha (+)/izquierda (-), z= abajo (+)/arriba (-))
	//IMPORTANTE - COMENTADA LA IA, descomentar la creacion y el add del componente IA en "CreateEnemy", "CreateDemon" y "CreateAngel"

	//ZONA 1 -> AMP. CITY
    //Primer pasillo (Donde nace DEX)
    createEnemy(vec3(5,0,-32), vec3(1), vector<vec3>(0), ++scheduling_AI_counter);
    //Segundo pasillo (al girar a la izquierda)
    createEnemy(vec3(-22,0,-28), vec3(1), vector<vec3>(0), ++scheduling_AI_counter);
    createEnemy(vec3(-40,0,-33), vec3(1), vector<vec3>(0), ++scheduling_AI_counter);
    createEnemy(vec3(-50,0,-27), vec3(1), vector<vec3>(0), ++scheduling_AI_counter);
    //Tercer pasillo (girar a la derecha)
    createEnemy(vec3(-77,0,-23), vec3(1), vector<vec3>(0), ++scheduling_AI_counter);
    createEnemy(vec3(-80,0,-35), vec3(1), vector<vec3>(0), ++scheduling_AI_counter);
    createEnemy(vec3(-87,0,-27), vec3(1), vector<vec3>(0), ++scheduling_AI_counter);
    //Puente 1 (Antes de abrir la puerta con las llaves)
    createEnemy(vec3(-72,0,-65), vec3(1), vector<vec3>(0), ++scheduling_AI_counter);
    createEnemy(vec3(-83,0,-75), vec3(1), vector<vec3>(0), ++scheduling_AI_counter);
    createEnemy(vec3(-75,0,-85), vec3(1), vector<vec3>(0), ++scheduling_AI_counter);
    createEnemy(vec3(-87,0,-100), vec3(1), vector<vec3>(0), ++scheduling_AI_counter);
    createEnemy(vec3(-74,0,-102), vec3(1), vector<vec3>(0), ++scheduling_AI_counter);
    createEnemy(vec3(-82,0,-113), vec3(1), vector<vec3>(0), ++scheduling_AI_counter);
    createEnemy(vec3(-77,0,-125), vec3(1), vector<vec3>(0), ++scheduling_AI_counter);
    createEnemy(vec3(-88,0,-132), vec3(1), vector<vec3>(0), ++scheduling_AI_counter);
    createEnemy(vec3(-71,0,-146), vec3(1), vector<vec3>(0), ++scheduling_AI_counter);
    createEnemy(vec3(-85,0,-153), vec3(1), vector<vec3>(0), ++scheduling_AI_counter);
    createEnemy(vec3(-79,0,-156), vec3(1), vector<vec3>(0), ++scheduling_AI_counter);
    createAngel(vec3(-82,0,-168), vec3(1), vector<vec3>(0), ++scheduling_AI_counter);
    createEnemy(vec3(-87,0,-180), vec3(1), vector<vec3>(0), ++scheduling_AI_counter);
    createEnemy(vec3(-80,0,-190), vec3(1), vector<vec3>(0), ++scheduling_AI_counter);
    createEnemy(vec3(-75,0,-205), vec3(1), vector<vec3>(0), ++scheduling_AI_counter);
    createEnemy(vec3(-83,0,-210), vec3(1), vector<vec3>(0), ++scheduling_AI_counter);
    createDemon(vec3(-83,0,-220), vec3(1), vector<vec3>(0), ++scheduling_AI_counter);
    createEnemy(vec3(-85,0,-230), vec3(1), vector<vec3>(0), ++scheduling_AI_counter);
    createEnemy(vec3(-76,0,-235), vec3(1), vector<vec3>(0), ++scheduling_AI_counter);
    createAngel(vec3(-85,0,-246), vec3(1), vector<vec3>(0), ++scheduling_AI_counter);
    createDemon(vec3(-75,0,-251), vec3(1), vector<vec3>(0), ++scheduling_AI_counter);
    //Rellano entre zona Angelical y demoniaca (Justo despues del primer puente).
    createEnemy(vec3(-95,0,-260), vec3(1), vector<vec3>(0), ++scheduling_AI_counter);
    createEnemy(vec3(-105,0,-253), vec3(1), vector<vec3>(0), ++scheduling_AI_counter);
    createEnemy(vec3(-100,0,-273), vec3(1), vector<vec3>(0), ++scheduling_AI_counter);
    createDemon(vec3(-94,0,-291), vec3(1), vector<vec3>(0), ++scheduling_AI_counter);
    createAngel(vec3(-110,0,-283), vec3(1), vector<vec3>(0), ++scheduling_AI_counter);
    createDemon(vec3(-74,0,-285), vec3(1), vector<vec3>(0), ++scheduling_AI_counter);
    createEnemy(vec3(-82,0,-270), vec3(1), vector<vec3>(0), ++scheduling_AI_counter);

    //ZONA 2 -> CATEDRAL IGLESIA
    //Pasillo Principal
    createEnemy(vec3(-142,0,-285), vec3(1), vector<vec3>(0), ++scheduling_AI_counter);
    createAngel(vec3(-152,0,-270), vec3(1), vector<vec3>(0), ++scheduling_AI_counter);
    createEnemy(vec3(-165,0,-270), vec3(1), vector<vec3>(0), ++scheduling_AI_counter);
    createAngel(vec3(-178,0,-262), vec3(1), vector<vec3>(0), ++scheduling_AI_counter);
    createEnemy(vec3(-186,0,-258), vec3(1), vector<vec3>(0), ++scheduling_AI_counter);
    createAngel(vec3(-193,0,-285), vec3(1), vector<vec3>(0), ++scheduling_AI_counter);
    createEnemy(vec3(-200,0,-270), vec3(1), vector<vec3>(0), ++scheduling_AI_counter);
    createEnemy(vec3(-207,0,-285), vec3(1), vector<vec3>(0), ++scheduling_AI_counter);
    createAngel(vec3(-225,0,-283), vec3(1), vector<vec3>(0), ++scheduling_AI_counter);
    createAngel(vec3(-225,0,-260), vec3(1), vector<vec3>(0), ++scheduling_AI_counter);
    createEnemy(vec3(-230,0,-268), vec3(1), vector<vec3>(0), ++scheduling_AI_counter);
    createEnemy(vec3(-230,0,-278), vec3(1), vector<vec3>(0), ++scheduling_AI_counter);
    createAngel(vec3(-243,0,-272), vec3(1), vector<vec3>(0), ++scheduling_AI_counter);
    createEnemy(vec3(-262,0,-284), vec3(1), vector<vec3>(0), ++scheduling_AI_counter);
    createAngel(vec3(-262,0,-272), vec3(1), vector<vec3>(0), ++scheduling_AI_counter);
    createEnemy(vec3(-262,0,-260), vec3(1), vector<vec3>(0), ++scheduling_AI_counter);
    //Zona Inferior (mesa con velas)
    createEnemy(vec3(-246,0,-250), vec3(1), vector<vec3>(0), ++scheduling_AI_counter);
    createEnemy(vec3(-258,0,-240), vec3(1), vector<vec3>(0), ++scheduling_AI_counter);
    createAngel(vec3(-246,0,-240), vec3(1), vector<vec3>(0), ++scheduling_AI_counter);
    createEnemy(vec3(-232,0,-240), vec3(1), vector<vec3>(0), ++scheduling_AI_counter);
    createEnemy(vec3(-245,0,-232), vec3(1), vector<vec3>(0), ++scheduling_AI_counter);
    //Zona Superior (Organo/Piano pa la musiquita jijiji)
    createEnemy(vec3(-246,0,-315), vec3(1), vector<vec3>(0), ++scheduling_AI_counter);
    createEnemy(vec3(-258,0,-305), vec3(1), vector<vec3>(0), ++scheduling_AI_counter);
    createAngel(vec3(-246,0,-305), vec3(1), vector<vec3>(0), ++scheduling_AI_counter);
    createEnemy(vec3(-232,0,-305), vec3(1), vector<vec3>(0), ++scheduling_AI_counter);
    createEnemy(vec3(-245,0,-297), vec3(1), vector<vec3>(0), ++scheduling_AI_counter);
    //Altar (Donde esta la llave)
    createEnemy(vec3(-277,0,-263), vec3(1), vector<vec3>(0), ++scheduling_AI_counter);
    createEnemy(vec3(-277,0,-280), vec3(1), vector<vec3>(0), ++scheduling_AI_counter);
    createEnemy(vec3(-288,0,-271), vec3(1), vector<vec3>(0), ++scheduling_AI_counter);
    createAngel(vec3(-303,0,-272), vec3(1), vector<vec3>(0), ++scheduling_AI_counter);

    //ZONA 3 -> ZONA DE DEMONIOS (PASILLO + ZONA PENTAGRAMA)
    //Pasillo PT1 (Antes de llegar a la zona pentagrama)
    createEnemy(vec3(-55,0,-270), vec3(1), vector<vec3>(0), ++scheduling_AI_counter);
    createEnemy(vec3(-50,0,-265), vec3(1), vector<vec3>(0), ++scheduling_AI_counter);
    createAngel(vec3(-42,0,-275), vec3(1), vector<vec3>(0), ++scheduling_AI_counter);
    createEnemy(vec3(-25,0,-267), vec3(1), vector<vec3>(0), ++scheduling_AI_counter);
    createDemon(vec3(-15,0,-279), vec3(1), vector<vec3>(0), ++scheduling_AI_counter);
    createEnemy(vec3(-7,0,-274), vec3(1), vector<vec3>(0), ++scheduling_AI_counter);
    createEnemy(vec3(-2,0,-267), vec3(1), vector<vec3>(0), ++scheduling_AI_counter);
    //Cruce de caminos(caminos cortados arriba y debajo)
    createAngel(vec3(8,0,-287), vec3(1), vector<vec3>(0), ++scheduling_AI_counter);
    createEnemy(vec3(18,0,-282), vec3(1), vector<vec3>(0), ++scheduling_AI_counter);
    createDemon(vec3(7,0,-257), vec3(1), vector<vec3>(0), ++scheduling_AI_counter);
    createEnemy(vec3(14,0,-254), vec3(1), vector<vec3>(0), ++scheduling_AI_counter);
    //Pasillo PT2
    createEnemy(vec3(27,0,-272), vec3(1), vector<vec3>(0), ++scheduling_AI_counter);
    createDemon(vec3(41,0,-278), vec3(1), vector<vec3>(0), ++scheduling_AI_counter);
    createDemon(vec3(41,0,-264), vec3(1), vector<vec3>(0), ++scheduling_AI_counter);
    createAngel(vec3(55,0,-271), vec3(1), vector<vec3>(0), ++scheduling_AI_counter);
    createEnemy(vec3(68,0,-267), vec3(1), vector<vec3>(0), ++scheduling_AI_counter);

    // PAUSE

	createMenuOptions(0, NUM_PAUSE_OPTIONS);


	createTexture(PAUSE_CONTINUE, 0, 0);
	createTexture(PAUSE_OPTIONS, 0, 0);
	createTexture(PAUSE_EXIT, 0, 0);

	createTexture(PAUSE_TEXTURE, 0, 0);

/*
	std::cout << Transformable::getName()     << " " << componentStorage.getComponents<Transformable>().size() << "\n";
	std::cout << Physics::getName()           << " " << componentStorage.getComponents<Physics>().size() << "\n";
	std::cout << RigidStaticAABB::getName()   << " " << componentStorage.getComponents<RigidStaticAABB>().size() << "\n";
	std::cout << TriggerMovSphere::getName()  << " " << componentStorage.getComponents<TriggerMovSphere>().size() << "\n";
	std::cout << AI::getName()                << " " << componentStorage.getComponents<AI>().size() << "\n";
	std::cout << TriggerStaticAABB::getName() << " " << componentStorage.getComponents<TriggerStaticAABB>().size() << "\n";
	std::cout << Render::getName()            << " " << componentStorage.getComponents<Render>().size() << "\n";
	std::cout << Velocity::getName()          << " " << componentStorage.getComponents<Velocity>().size() << "\n";
	std::cout << CharacterData::getName()     << " " << componentStorage.getComponents<CharacterData>().size() << "\n";
*/
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
	auto& node_11 = graph.emplace_back(MapNode(-220, -340));
	auto& node_12 = graph.emplace_back(MapNode(-220, -250));
	auto& node_13 = graph.emplace_back(MapNode(-310, -220));
	auto& node_14 = graph.emplace_back(MapNode(-310, -340));

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

void EntityManager::createIntro() {
	initData(8, 0, 8);

	createMenuOptions(0, 4);

	createVideo("resources/videos/intro/1_F.mp4", false);
	createVideo("resources/videos/intro/2_F_L.mp4", true);

	createTexture("resources/menu/main_menu/op_menu_1.png", 0, 0);
	createTexture("resources/menu/main_menu/op_menu_2.png", 0, 0);
	createTexture("resources/menu/main_menu/op_menu_3.png", 0, 0);
	createTexture("resources/menu/main_menu/op_menu_4.png", 0, 0);


	std::cout << Video::getName()     << " " << componentStorage.getComponents<Video>().size() << "\n";
	std::cout << TextureCmp::getName()     << " " << componentStorage.getComponents<TextureCmp>().size() << "\n";

}

void EntityManager::nextVideo() {
	idxVideo++;
}