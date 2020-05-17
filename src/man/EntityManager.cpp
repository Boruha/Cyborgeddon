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
	graph.reserve(165);
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

	render.node = componentStorage.createAnimatedMesh("../resources/models/Dex/Walking");

	render.node->setPosition(physics.position);
	render.node->setRotation(physics.rotation);
	render.node->setScale(physics.scale);

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

	render.node = componentStorage.createMesh("../resources/models/Cubo/cuboPrueba.fbx");

	render.node->setPosition(transformable.position);
	render.node->setRotation(transformable.rotation);
	render.node->setScale(transformable.scale / 2.f);

	wall.addComponent(transformable);
	wall.addComponent(rigidStaticAABB);
	wall.addComponent(render);
}

void EntityManager::createEnemy(const vec3& pos, const vec3& dim, const std::vector<vec3>& patrol, unsigned phase) {
	auto& enemy     = createEntity(ENEMY);

	auto& physics   = componentStorage.createComponent(Physics(enemy.getType(), enemy.getID(), pos, vec3(), vec3(0.0, 180.0, 0.0), dim));
	auto& velocity  = componentStorage.createComponent(Velocity(enemy.getType(), enemy.getID(), ENEMY_SPEED, ENEMY_ACCELERATION));
	auto& trigger   = componentStorage.createComponent(TriggerMovSphere(enemy.getType(), enemy.getID(), physics.position, 1, physics.velocity));
	auto& circleBounding = componentStorage.createComponent(CircleBounding(enemy.getType(), enemy.getID(), physics.position, 1.f, physics.velocity));
	auto& data      = componentStorage.createComponent(CharacterData(enemy.getType(), enemy.getID(), NEUTRAL, ENEMY_HEALTH, ENEMY_SWITCH_MODE_COOLDOWN, ENEMY_ATTACK_DAMAGE, ENEMY_ATTACKING_COOLDOWN, MELEE_ATTACK_RANGE2, ENEMY_DASH_SPEED, ENEMY_DASH_COOLDOWN));
	auto& ai        = componentStorage.createComponent(AI(enemy.getType(), enemy.getID(), patrol, phase));
	auto& render	= componentStorage.createComponent(Render(enemy.getType(), enemy.getID(), &physics.position, &physics.rotation, &physics.scale, true));

	render.node = componentStorage.createAnimatedMesh("../resources/models/Bot/Static");

	render.node->setPosition(physics.position);
	render.node->setRotation(physics.rotation);
	render.node->setScale(physics.scale);

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
	auto& trigger   = componentStorage.createComponent(TriggerMovSphere(enemy.getType(), enemy.getID(), physics.position, 1, physics.velocity));
	auto& circleBounding = componentStorage.createComponent(CircleBounding(enemy.getType(), enemy.getID(), physics.position, 1.f, physics.velocity));
	auto& data      = componentStorage.createComponent(CharacterData(enemy.getType(), enemy.getID(), ANGEL, ENEMY_HEALTH, ENEMY_SWITCH_MODE_COOLDOWN, ENEMY_ATTACK_DAMAGE, ENEMY_ATTACKING_COOLDOWN, DIST_ATTACK_RANGE2, ENEMY_DASH_SPEED, ENEMY_DASH_COOLDOWN));
	auto& ai        = componentStorage.createComponent(AI(enemy.getType(), enemy.getID(), patrol, phase));
	auto& render	= componentStorage.createComponent(Render(enemy.getType(), enemy.getID(), &physics.position, &physics.rotation, &physics.scale, true));

	render.node = componentStorage.createAnimatedMesh("../resources/models/Angel/Static");

	render.node->setPosition(physics.position);
	render.node->setRotation(physics.rotation);
	render.node->setScale(physics.scale);

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
	auto& trigger   = componentStorage.createComponent(TriggerMovSphere(enemy.getType(), enemy.getID(), physics.position, 1, physics.velocity));
	auto& circleBounding = componentStorage.createComponent(CircleBounding(enemy.getType(), enemy.getID(), physics.position, 1.f, physics.velocity));
	auto& data      = componentStorage.createComponent(CharacterData(enemy.getType(), enemy.getID(), DEMON, ENEMY_HEALTH, ENEMY_SWITCH_MODE_COOLDOWN, ENEMY_ATTACK_DAMAGE, ENEMY_ATTACKING_COOLDOWN, MELEE_ATTACK_RANGE2, ENEMY_DASH_SPEED, ENEMY_DASH_COOLDOWN));
	auto& jump 		= componentStorage.createComponent(Jump(enemy.getType(), enemy.getID(), JUMP_COOLDOWN, JUMP_TIMER));
	auto& ai        = componentStorage.createComponent(AI(enemy.getType(), enemy.getID(), patrol, phase));
	auto& render	= componentStorage.createComponent(Render(enemy.getType(), enemy.getID(), &physics.position, &physics.rotation, &physics.scale, true));

	render.node = componentStorage.createAnimatedMesh("../resources/models/Demon/Static");


	render.node->setPosition(physics.position);
	render.node->setRotation(physics.rotation);
	render.node->setScale(physics.scale);

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

	render.node = componentStorage.createMesh("../resources/models/Ciudad/ciudad.obj");

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

	render.node = componentStorage.createMesh("../resources/models/Cubo/cuboPrueba.fbx");

	render.node->setPosition(physics.position);
	render.node->setRotation(physics.rotation);
	render.node->setScale(physics.scale);

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

	render.node = componentStorage.createMesh("../resources/models/Objetos/Puerta/0.obj");

	render.node->setPosition(transformable.position);
	render.node->setRotation(transformable.rotation);
	render.node->setScale(transformable.scale);

	door.addComponent(transformable);
    door.addComponent(trigger);
	door.addComponent(rigid);
	door.addComponent(render);

	Entity& key 		    = createEntity(KEY);

	auto& keyTransformable  = componentStorage.createComponent(Transformable(key.getType(), key.getID(), keyPos + vec3(0, 0, 0), vec3(), keyDim));
	auto& keyTrigger        = componentStorage.createComponent(TriggerStaticAABB(key.getType(), key.getID(), keyTransformable.position, keyTransformable.scale, true));
	auto& keyRender			= componentStorage.createComponent(Render(key.getType(), key.getID(), &keyTransformable.position, &keyTransformable.rotation, &keyTransformable.scale, false));

	keyRender.node = componentStorage.createMesh("../resources/models/Objetos/Llave_demoniaca/0.obj");

	keyRender.node->setPosition(keyTransformable.position);
	keyRender.node->setRotation(keyTransformable.rotation);
	keyRender.node->setScale(keyTransformable.scale);

	key.addComponent(keyTransformable);
	key.addComponent(keyTrigger);
	key.addComponent(keyRender);
}

void EntityManager::createVideo(const std::string_view path, const bool isLoop, const SoundParameter sound) {
	auto& video = createEntity(VIDEO);

	auto& cmpVideo = componentStorage.createComponent(Video(video.getType(), video.getID(), path));

	cmpVideo.video = engine.loadVideo(path);

	cmpVideo.frameCounter = 0;
	cmpVideo.numFrames = unsigned(cmpVideo.video->getNumFrames());
	cmpVideo.timePerFrame = float(1.f / cmpVideo.video->getFPS());

	cmpVideo.loop = isLoop;

	cmpVideo.video->setLoop(cmpVideo.loop);

	cmpVideo.sound = sound;

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
	
	createPairPlayerCamera(vec3(), vec3(1.f), vec3(5, 40, 10)); //x= , y= , z=    <------> antes: vec3(30, 120, 70) - (10, 50, 10)
	createLight(vec3(-40, 80, -60), vec3(-11.0, -22.0, 8.0), vec3(0.8), vec3(0.0));

	createFloor(CONTROLS_TEXTURE, vec3(0,0,0), vec3(0,0,0)); //Controls
	readColliderFile("../resources/models/Ciudad/colisiones.obj");

	createTexture(KEY_2, 558, 52);
	createTexture(KEY_1, 517, 52);
	createTexture(MAIN_HUD, 0, 35);
	createTexture(ANGEL_LIFEBAR, 0, 86);
	createTexture(DEMON_LIFEBAR, 0, 86);
	createTexture(BACKGROUND_LIFEBAR, 0, 78);

	createGraph();
	//puntero a vec3? Mejor?

	//NOTACION -> patrol_numeroDeNodo -> asignar a enemigos
    //ZONA 1 -> AMP. CITY
    std::vector<vec3> patrol_0 = { graph[0].coord };
	std::vector<vec3> patrol_1 = { graph[1].coord };
	std::vector<vec3> patrol_2 = { graph[2].coord };
	std::vector<vec3> patrol_3 = { graph[3].coord };
	std::vector<vec3> patrol_4 = { graph[4].coord };
	std::vector<vec3> patrol_5 = { graph[5].coord };
	std::vector<vec3> patrol_6 = { graph[6].coord, graph[7].coord };
    //Puente 1 (Antes de abrir la puerta con las llaves)
	std::vector<vec3> patrol_8 = { graph[8].coord};
	std::vector<vec3> patrol_9 = { graph[9].coord};
	std::vector<vec3> patrol_11 = { graph[11].coord, graph[10].coord };
	std::vector<vec3> patrol_12 = { graph[12].coord };
    std::vector<vec3> patrol_13 = { graph[13].coord };
    std::vector<vec3> patrol_14 = { graph[14].coord, graph[15].coord };
    std::vector<vec3> patrol_16 = { graph[16].coord, graph[17].coord };
    std::vector<vec3> patrol_18 = { graph[18].coord };
    std::vector<vec3> patrol_19 = { graph[19].coord };
    std::vector<vec3> patrol_21 = { graph[21].coord, graph[20].coord };
    std::vector<vec3> patrol_22 = { graph[22].coord };
    std::vector<vec3> patrol_23 = { graph[23].coord };
    std::vector<vec3> patrol_24 = { graph[24].coord };
    //Rellano entre zona Angelical y demoniaca (Justo despues del primer puente).
    std::vector<vec3> patrol_26 = { graph[26].coord, graph[25].coord };
    std::vector<vec3> patrol_27 = { graph[27].coord };
    std::vector<vec3> patrol_28 = { graph[28].coord };
    std::vector<vec3> patrol_29 = { graph[29].coord };
    std::vector<vec3> patrol_30 = { graph[30].coord };
    std::vector<vec3> patrol_32 = { graph[32].coord, graph[31].coord };
    //ZONA 2 -> CATEDRAL IGLESIA
    //Pasillo Principal
    std::vector<vec3> patrol_34 = { graph[34].coord };
    std::vector<vec3> patrol_35 = { graph[35].coord, graph[33].coord };
    std::vector<vec3> patrol_36 = { graph[36].coord };
    std::vector<vec3> patrol_37 = { graph[37].coord };
    std::vector<vec3> patrol_38 = { graph[38].coord };
    std::vector<vec3> patrol_40 = { graph[40].coord };
    std::vector<vec3> patrol_41 = { graph[41].coord, graph[39].coord };
    std::vector<vec3> patrol_42 = { graph[42].coord };
    std::vector<vec3> patrol_43 = { graph[43].coord };
    std::vector<vec3> patrol_44 = { graph[44].coord };
    std::vector<vec3> patrol_46 = { graph[46].coord, graph[45].coord };
    std::vector<vec3> patrol_47 = { graph[47].coord, graph[48].coord };
    //Zona Inferior (mesa con velas)
    std::vector<vec3> patrol_49 = { graph[49].coord };
    std::vector<vec3> patrol_50 = { graph[50].coord };
    std::vector<vec3> patrol_51 = { graph[51].coord };
    std::vector<vec3> patrol_52 = { graph[52].coord };
    //Zona Superior (Organo/Piano pa la musiquita jijiji)
    std::vector<vec3> patrol_53 = { graph[53].coord };
    std::vector<vec3> patrol_54 = { graph[54].coord };
    std::vector<vec3> patrol_55 = { graph[55].coord };
    std::vector<vec3> patrol_56 = { graph[56].coord };
    //Altar (Donde esta la llave)
    std::vector<vec3> patrol_57 = { graph[57].coord };
    std::vector<vec3> patrol_58 = { graph[58].coord };
    std::vector<vec3> patrol_59 = { graph[59].coord };
    //ZONA 3 -> ZONA DE DEMONIOS (PASILLO + ZONA PENTAGRAMA)
    //Carretera de camino
    std::vector<vec3> patrol_60 = { graph[60].coord, graph[61].coord};
    std::vector<vec3> patrol_62 = { graph[62].coord };
    std::vector<vec3> patrol_63 = { graph[63].coord };
    std::vector<vec3> patrol_64 = { graph[64].coord };
    std::vector<vec3> patrol_65 = { graph[65].coord, graph[66].coord };
    std::vector<vec3> patrol_67 = { graph[67].coord };
    std::vector<vec3> patrol_68 = { graph[68].coord };
    std::vector<vec3> patrol_70 = { graph[70].coord, graph[69].coord };
    std::vector<vec3> patrol_71 = { graph[71].coord };
    std::vector<vec3> patrol_73 = { graph[73].coord, graph[72].coord};
    std::vector<vec3> patrol_74 = { graph[74].coord };
    std::vector<vec3> patrol_75 = { graph[75].coord };
    std::vector<vec3> patrol_76 = { graph[76].coord };
    //Zona demoniaca (Pentagrama)
    std::vector<vec3> patrol_77 = { graph[77].coord };
    std::vector<vec3> patrol_78 = { graph[78].coord };
    std::vector<vec3> patrol_79 = { graph[79].coord };
    std::vector<vec3> patrol_80 = { graph[80].coord };
    std::vector<vec3> patrol_81 = { graph[81].coord };
    std::vector<vec3> patrol_82 = { graph[82].coord };
    std::vector<vec3> patrol_83 = { graph[83].coord };
    std::vector<vec3> patrol_84 = { graph[84].coord };
    std::vector<vec3> patrol_85 = { graph[85].coord };
    std::vector<vec3> patrol_86 = { graph[86].coord };
    std::vector<vec3> patrol_87 = { graph[87].coord };
    std::vector<vec3> patrol_88 = { graph[88].coord };
    std::vector<vec3> patrol_89 = { graph[89].coord };
    std::vector<vec3> patrol_90 = { graph[90].coord };
    std::vector<vec3> patrol_91 = { graph[91].coord };
    std::vector<vec3> patrol_92 = { graph[92].coord };
    std::vector<vec3> patrol_93 = { graph[93].coord };
    std::vector<vec3> patrol_94 = { graph[94].coord };
    std::vector<vec3> patrol_95 = { graph[95].coord };
    std::vector<vec3> patrol_96 = { graph[96].coord };
    std::vector<vec3> patrol_97 = { graph[97].coord, graph[98].coord };
    std::vector<vec3> patrol_98 = { graph[98].coord, graph[99].coord };
    std::vector<vec3> patrol_99 = { graph[99].coord, graph[100].coord };
    std::vector<vec3> patrol_100 = { graph[100].coord, graph[101].coord };
    std::vector<vec3> patrol_101 = { graph[101].coord, graph[97].coord };
    //ZONA 4 -> Zona del caos (Zona final)
    std::vector<vec3> patrol_102 = { graph[102].coord };
    std::vector<vec3> patrol_103 = { graph[103].coord };
    std::vector<vec3> patrol_106 = { graph[106].coord };
    std::vector<vec3> patrol_108 = { graph[108].coord, graph[109].coord, graph[110].coord, graph[111].coord };
    std::vector<vec3> patrol_112 = { graph[112].coord, graph[113].coord };
    std::vector<vec3> patrol_117 = { graph[117].coord };
    std::vector<vec3> patrol_120 = { graph[120].coord, graph[119].coord };
    std::vector<vec3> patrol_121 = { graph[121].coord };
    std::vector<vec3> patrol_122 = { graph[122].coord };
    std::vector<vec3> patrol_127 = { graph[127].coord, graph[126].coord };
    std::vector<vec3> patrol_128 = { graph[128].coord };
    std::vector<vec3> patrol_132 = { graph[132].coord, graph[131].coord, graph[129].coord, graph[130].coord };
    std::vector<vec3> patrol_137 = { graph[137].coord };
    std::vector<vec3> patrol_138 = { graph[138].coord };
    std::vector<vec3> patrol_139 = { graph[139].coord };
    std::vector<vec3> patrol_143 = { graph[143].coord };
    std::vector<vec3> patrol_145 = { graph[145].coord, graph[146].coord, graph[148].coord, graph[147].coord };
    std::vector<vec3> patrol_149 = { graph[149].coord, graph[150].coord };
    std::vector<vec3> patrol_154 = { graph[154].coord, graph[155].coord, graph[156].coord, graph[157].coord };
    std::vector<vec3> patrol_158 = { graph[158].coord };
    std::vector<vec3> patrol_162 = { graph[162].coord };

	unsigned scheduling_AI_counter = 0;

	//CREACION DE LAS LLAVES
    //createPairKeyDoor(vec3(0,0,-20), vec3(1), vec3(0,0,-10), vec3(1));


	//DISTRIBUCION DE ENEMIGOS -> (x=derecha (+)/izquierda (-), z= abajo (+)/arriba (-))
	//IMPORTANTE - COMENTADA LA IA, descomentar la creacion y el add del componente IA en "CreateEnemy", "CreateDemon" y "CreateAngel"

    //ZONA 1 -> AMP. CITY
    //Primer pasillo (Donde nace DEX)
    createEnemy(patrol_0[0], vec3(1), patrol_0, ++scheduling_AI_counter);
    //Segundo pasillo (al girar a la izquierda)
    createEnemy(patrol_1[0], vec3(1), patrol_1, ++scheduling_AI_counter);
    createEnemy(patrol_2[0], vec3(1), patrol_2, ++scheduling_AI_counter);
    createEnemy(patrol_3[0], vec3(1), patrol_3, ++scheduling_AI_counter);
    //Tercer pasillo (girar a la derecha)
    createEnemy(patrol_4[0], vec3(1), patrol_4, ++scheduling_AI_counter);
    createEnemy(patrol_6[0], vec3(1), patrol_6, ++scheduling_AI_counter);
    createEnemy(patrol_5[0], vec3(1), patrol_5, ++scheduling_AI_counter);
    //Puente 1 (Antes de abrir la puerta con las llaves)
    createEnemy(patrol_8[0], vec3(1), patrol_8, ++scheduling_AI_counter);
    createEnemy(patrol_9[0], vec3(1), patrol_9, ++scheduling_AI_counter);
    createEnemy(patrol_11[0], vec3(1), patrol_11, ++scheduling_AI_counter);
    createEnemy(patrol_12[0], vec3(1), patrol_12, ++scheduling_AI_counter);
    createEnemy(patrol_13[0], vec3(1), patrol_13, ++scheduling_AI_counter);
    createEnemy(patrol_14[0], vec3(1), patrol_14, ++scheduling_AI_counter);
    createEnemy(patrol_16[0], vec3(1), patrol_16, ++scheduling_AI_counter);
    createAngel(patrol_18[0], vec3(1), patrol_18, ++scheduling_AI_counter);
    createEnemy(patrol_19[0], vec3(1), patrol_19, ++scheduling_AI_counter);
    createEnemy(patrol_21[0], vec3(1), patrol_21, ++scheduling_AI_counter);
    createDemon(patrol_22[0], vec3(1), patrol_22, ++scheduling_AI_counter);
    createAngel(patrol_23[0], vec3(1), patrol_23, ++scheduling_AI_counter);
    createDemon(patrol_24[0], vec3(1), patrol_24, ++scheduling_AI_counter);
    //Rellano entre zona Angelical y demoniaca (Justo despues del primer puente).
    createEnemy(patrol_26[0], vec3(1), patrol_26, ++scheduling_AI_counter);
    createAngel(patrol_28[0], vec3(1), patrol_28, ++scheduling_AI_counter);
    createEnemy(patrol_32[0], vec3(1), patrol_32, ++scheduling_AI_counter);
    createEnemy(patrol_30[0], vec3(1), patrol_30, ++scheduling_AI_counter);
    createDemon(patrol_29[0], vec3(1), patrol_29, ++scheduling_AI_counter);
    createEnemy(patrol_27[0], vec3(1), patrol_27, ++scheduling_AI_counter);

    //ZONA 2 -> CATEDRAL IGLESIA
    //Pasillo Principal
    createEnemy(patrol_34[0], vec3(1), patrol_34, ++scheduling_AI_counter);
    createAngel(patrol_35[0], vec3(1), patrol_35, ++scheduling_AI_counter);
    createEnemy(patrol_36[0], vec3(1), patrol_36, ++scheduling_AI_counter);
    createEnemy(patrol_37[0], vec3(1), patrol_37, ++scheduling_AI_counter);
    createEnemy(patrol_38[0], vec3(1), patrol_38, ++scheduling_AI_counter);
    createEnemy(patrol_41[0], vec3(1), patrol_41, ++scheduling_AI_counter);
    createEnemy(patrol_42[0], vec3(1), patrol_42, ++scheduling_AI_counter);
    createAngel(patrol_40[0], vec3(1), patrol_40, ++scheduling_AI_counter);
    createAngel(patrol_43[0], vec3(1), patrol_43, ++scheduling_AI_counter);
    createAngel(patrol_44[0], vec3(1), patrol_44, ++scheduling_AI_counter);
    createEnemy(patrol_46[0], vec3(1), patrol_46, ++scheduling_AI_counter);
    createEnemy(patrol_47[0], vec3(1), patrol_47, ++scheduling_AI_counter);
    //Zona Inferior (mesa con velas)
    createEnemy(patrol_49[0], vec3(1), patrol_49, ++scheduling_AI_counter);
    createAngel(patrol_50[0], vec3(1), patrol_50, ++scheduling_AI_counter);
    createEnemy(patrol_51[0], vec3(1), patrol_51, ++scheduling_AI_counter);
    createEnemy(patrol_52[0], vec3(1), patrol_52, ++scheduling_AI_counter);
    //Zona Superior (Organo/Piano pa la musiquita jijiji)
    createAngel(patrol_56[0], vec3(1), patrol_56, ++scheduling_AI_counter);
    createEnemy(patrol_53[0], vec3(1), patrol_53, ++scheduling_AI_counter);
    createEnemy(patrol_55[0], vec3(1), patrol_55, ++scheduling_AI_counter);
    createEnemy(patrol_54[0], vec3(1), patrol_54, ++scheduling_AI_counter);
    //Altar (Donde esta la llave)
    createEnemy(patrol_59[0], vec3(1), patrol_59, ++scheduling_AI_counter);
    createEnemy(patrol_57[0], vec3(1), patrol_57, ++scheduling_AI_counter);
    createAngel(patrol_58[0], vec3(1), patrol_58, ++scheduling_AI_counter);

    //ZONA 3 -> ZONA DE DEMONIOS (PASILLO + ZONA PENTAGRAMA)
    //Pasillo PT1 (Antes de llegar a la zona pentagrama)
    createEnemy(patrol_60[0], vec3(1), patrol_60, ++scheduling_AI_counter);
    createEnemy(patrol_62[0], vec3(1), patrol_62, ++scheduling_AI_counter);
    createEnemy(patrol_63[0], vec3(1), patrol_63, ++scheduling_AI_counter);
    createDemon(patrol_64[0], vec3(1), patrol_64, ++scheduling_AI_counter);
    createEnemy(patrol_65[0], vec3(1), patrol_65, ++scheduling_AI_counter);
    //Cruce de caminos(caminos cortados arriba y debajo)
    createAngel(patrol_67[0], vec3(1), patrol_67, ++scheduling_AI_counter);
    createEnemy(patrol_68[0], vec3(1), patrol_68, ++scheduling_AI_counter);
    createDemon(patrol_70[0], vec3(1), patrol_70, ++scheduling_AI_counter);
    createEnemy(patrol_71[0], vec3(1), patrol_71, ++scheduling_AI_counter);
    //Pasillo PT2
    createEnemy(patrol_73[0], vec3(1), patrol_73, ++scheduling_AI_counter);
    createEnemy(patrol_74[0], vec3(1), patrol_74, ++scheduling_AI_counter);
    createDemon(patrol_75[0], vec3(1), patrol_75, ++scheduling_AI_counter);
    createDemon(patrol_76[0], vec3(1), patrol_76, ++scheduling_AI_counter);

    //Zona demoniaca (Pentagrama) (Primera mitad)
    createDemon(patrol_77[0], vec3(1), patrol_77, ++scheduling_AI_counter);
    createEnemy(patrol_78[0], vec3(1), patrol_78, ++scheduling_AI_counter);
    createEnemy(patrol_79[0], vec3(1), patrol_79, ++scheduling_AI_counter);
    createEnemy(patrol_80[0], vec3(1), patrol_80, ++scheduling_AI_counter);

    createDemon(patrol_81[0], vec3(1), patrol_81, ++scheduling_AI_counter);
    createEnemy(patrol_83[0], vec3(1), patrol_83, ++scheduling_AI_counter);
    createEnemy(patrol_84[0], vec3(1), patrol_84, ++scheduling_AI_counter);
    createEnemy(patrol_82[0], vec3(1), patrol_82, ++scheduling_AI_counter);

    createDemon(patrol_93[0], vec3(1), patrol_93, ++scheduling_AI_counter);
    createEnemy(patrol_94[0], vec3(1), patrol_94, ++scheduling_AI_counter);
    createEnemy(patrol_95[0], vec3(1), patrol_95, ++scheduling_AI_counter);
    createEnemy(patrol_96[0], vec3(1), patrol_96, ++scheduling_AI_counter);
    //Demonios alrededor de estatua gigante de Lucyborg
    createDemon(patrol_97[0], vec3(1), patrol_97, ++scheduling_AI_counter);
    createDemon(patrol_98[0], vec3(1), patrol_98, ++scheduling_AI_counter);
    createDemon(patrol_99[0], vec3(1), patrol_99, ++scheduling_AI_counter);
    createDemon(patrol_100[0], vec3(1), patrol_100, ++scheduling_AI_counter);
    createDemon(patrol_101[0], vec3(1), patrol_101, ++scheduling_AI_counter);
    //Zona demoniaca (Pentagrama) (Segunda mitad)
    createDemon(patrol_85[0], vec3(1), patrol_85, ++scheduling_AI_counter);
    createEnemy(patrol_87[0], vec3(1), patrol_87, ++scheduling_AI_counter);
    createEnemy(patrol_88[0], vec3(1), patrol_88, ++scheduling_AI_counter);
    createEnemy(patrol_86[0], vec3(1), patrol_86, ++scheduling_AI_counter);

    createEnemy(patrol_89[0], vec3(1), patrol_89, ++scheduling_AI_counter);
    createEnemy(patrol_91[0], vec3(1), patrol_91, ++scheduling_AI_counter);
    createDemon(patrol_90[0], vec3(1), patrol_90, ++scheduling_AI_counter);
    createEnemy(patrol_92[0], vec3(1), patrol_92, ++scheduling_AI_counter);

    //ZONA 4 -> Zona del caos (Zona final)
    //Bots al entrar
    createEnemy(patrol_158[0], vec3(1), patrol_158, ++scheduling_AI_counter);
    createEnemy(patrol_162[0], vec3(1), patrol_162, ++scheduling_AI_counter);
    createEnemy(patrol_103[0], vec3(1), patrol_103, ++scheduling_AI_counter);
    createEnemy(patrol_106[0], vec3(1), patrol_106, ++scheduling_AI_counter);
    createAngel(patrol_149[0], vec3(1), patrol_149, ++scheduling_AI_counter);
    //Bots del centro
    createEnemy(patrol_117[0], vec3(1), patrol_117, ++scheduling_AI_counter);
    createEnemy(patrol_122[0], vec3(1), patrol_122, ++scheduling_AI_counter);
    createEnemy(patrol_121[0], vec3(1), patrol_121, ++scheduling_AI_counter);
    createEnemy(patrol_128[0], vec3(1), patrol_128, ++scheduling_AI_counter);
    //Demonios alrededor del centro
    createDemon(patrol_120[0], vec3(1), patrol_120, ++scheduling_AI_counter);
    createDemon(patrol_112[0], vec3(1), patrol_112, ++scheduling_AI_counter);
    createDemon(patrol_143[0], vec3(1), patrol_143, ++scheduling_AI_counter);
    //Angeles alrededor del centro
    createAngel(patrol_127[0], vec3(1), patrol_127, ++scheduling_AI_counter);
    createAngel(patrol_102[0], vec3(1), patrol_102, ++scheduling_AI_counter);

    createDemon(patrol_154[0], vec3(1), patrol_154, ++scheduling_AI_counter);
    createDemon(patrol_145[0], vec3(1), patrol_145, ++scheduling_AI_counter);

    createAngel(patrol_108[0], vec3(1), patrol_108, ++scheduling_AI_counter);
    createAngel(patrol_132[0], vec3(1), patrol_132, ++scheduling_AI_counter);

    createAngel(patrol_139[0], vec3(1), patrol_139, ++scheduling_AI_counter);
    createEnemy(patrol_138[0], vec3(1), patrol_138, ++scheduling_AI_counter);
    createDemon(patrol_137[0], vec3(1), patrol_137, ++scheduling_AI_counter);


    // PAUSE
	createMenuOptions(0, NUM_PAUSE_OPTIONS);

	createTexture(PAUSE_CONTINUE, 0, 0);
	createTexture(PAUSE_OPTIONS, 0, 0);
	createTexture(PAUSE_EXIT, 0, 0);

	createTexture(PAUSE_TEXTURE, 0, 0);

	componentStorage.printComponentStorage();
}

void EntityManager::createGraph()
{
    //DECLARACION DE NODOS
    //ZONA 1 -> AMP. CITY
    auto& node_0  = graph.emplace_back(MapNode(5, -32));
    auto& node_1  = graph.emplace_back(MapNode(-22, -28));
    auto& node_2  = graph.emplace_back(MapNode(-40, -33));
    auto& node_3  = graph.emplace_back(MapNode(-50, -27));
    auto& node_4  = graph.emplace_back(MapNode(-77, -23));
    auto& node_5  = graph.emplace_back(MapNode(-87, -27));
    auto& node_6  = graph.emplace_back(MapNode(-80, -35));
    //Puente 1 (Antes de abrir la puerta con las llaves)
    auto& node_7  = graph.emplace_back(MapNode(-77, -50));
    auto& node_8  = graph.emplace_back(MapNode(-83, -75));
    auto& node_9  = graph.emplace_back(MapNode(-75, -85));
    auto& node_10  = graph.emplace_back(MapNode(-89, -86));
    auto& node_11  = graph.emplace_back(MapNode(-85, -100));
    auto& node_12  = graph.emplace_back(MapNode(-82, -113));
    auto& node_13  = graph.emplace_back(MapNode(-77, -125));
    auto& node_14  = graph.emplace_back(MapNode(-88, -135));
    auto& node_15  = graph.emplace_back(MapNode(-81, -141));
    auto& node_16  = graph.emplace_back(MapNode(-74, -148));
    auto& node_17  = graph.emplace_back(MapNode(-86, -153));
    auto& node_18  = graph.emplace_back(MapNode(-82, -171));
    auto& node_19  = graph.emplace_back(MapNode(-80, -190));
    auto& node_20  = graph.emplace_back(MapNode(-86, -204));
    auto& node_21  = graph.emplace_back(MapNode(-75, -205));
    auto& node_22  = graph.emplace_back(MapNode(-83, -220));
    auto& node_23  = graph.emplace_back(MapNode(-85, -246));
    auto& node_24  = graph.emplace_back(MapNode(-75, -251));
    //Rellano entre zona Angelical y demoniaca (Justo despues del primer puente).
    auto& node_25  = graph.emplace_back(MapNode(-105, -257));
    auto& node_26  = graph.emplace_back(MapNode(-95, -260));
    auto& node_27  = graph.emplace_back(MapNode(-82, -270));
    auto& node_28  = graph.emplace_back(MapNode(-100, -273));
    auto& node_29  = graph.emplace_back(MapNode(-70, -276));
    auto& node_30  = graph.emplace_back(MapNode(-110, -283));
    auto& node_31  = graph.emplace_back(MapNode(-93, -287));
    auto& node_32  = graph.emplace_back(MapNode(-76, -291));
    //ZONA 2 -> CATEDRAL IGLESIA
    //Pasillo Principal
    auto& node_33  = graph.emplace_back(MapNode(-128, -270));
    auto& node_34  = graph.emplace_back(MapNode(-142, -285));
    auto& node_35  = graph.emplace_back(MapNode(-152, -270));
    auto& node_36  = graph.emplace_back(MapNode(-165, -270));
    auto& node_37  = graph.emplace_back(MapNode(-178, -262));
    auto& node_38  = graph.emplace_back(MapNode(-186, -258));
    auto& node_39  = graph.emplace_back(MapNode(-184, -273));
    auto& node_40  = graph.emplace_back(MapNode(-193, -285));
    auto& node_41  = graph.emplace_back(MapNode(-200, -270));
    auto& node_42  = graph.emplace_back(MapNode(-207, -285));
    auto& node_43  = graph.emplace_back(MapNode(-230, -278));
    auto& node_44  = graph.emplace_back(MapNode(-230, -265));
    auto& node_45  = graph.emplace_back(MapNode(-257, -295));
    auto& node_46  = graph.emplace_back(MapNode(-257, -276));
    auto& node_47  = graph.emplace_back(MapNode(-257, -267));
    auto& node_48  = graph.emplace_back(MapNode(-257, -245));
    //Zona Inferior (mesa con velas)
    auto& node_49  = graph.emplace_back(MapNode(-245, -250));
    auto& node_50  = graph.emplace_back(MapNode(-245, -240));
    auto& node_51  = graph.emplace_back(MapNode(-258, -232));
    auto& node_52  = graph.emplace_back(MapNode(-232, -232));
    //Zona Superior (Organo/Piano pa la musiquita jijiji)
    auto& node_53  = graph.emplace_back(MapNode(-258, -305));
    auto& node_54  = graph.emplace_back(MapNode(-245, -297));
    auto& node_55  = graph.emplace_back(MapNode(-232, -305));
    auto& node_56  = graph.emplace_back(MapNode(-245, -318));
    //Altar (Donde esta la llave)
    auto& node_57  = graph.emplace_back(MapNode(-277, -280));
    auto& node_58  = graph.emplace_back(MapNode(-288, -271));
    auto& node_59  = graph.emplace_back(MapNode(-277, -263));
    //ZONA 3 -> ZONA DE DEMONIOS (PASILLO + ZONA PENTAGRAMA)
    //Pasillo (Antes de llegar a la zona pentagrama)
    auto& node_60  = graph.emplace_back(MapNode(-55, -270));
    auto& node_61  = graph.emplace_back(MapNode(-40, -267));
    auto& node_62  = graph.emplace_back(MapNode(-42, -275));
    auto& node_63  = graph.emplace_back(MapNode(-25, -267));
    auto& node_64  = graph.emplace_back(MapNode(-15, -279));
    auto& node_65  = graph.emplace_back(MapNode(-7, -274));
    auto& node_66  = graph.emplace_back(MapNode(6, -276));
    auto& node_67  = graph.emplace_back(MapNode(8, -287));
    auto& node_68  = graph.emplace_back(MapNode(18, -282));
    auto& node_69  = graph.emplace_back(MapNode(9, -269));
    auto& node_70  = graph.emplace_back(MapNode(7, -257));
    auto& node_71  = graph.emplace_back(MapNode(14, -254));
    auto& node_72  = graph.emplace_back(MapNode(24, -269));
    auto& node_73  = graph.emplace_back(MapNode(40, -274));
    auto& node_74  = graph.emplace_back(MapNode(48, -267));
    auto& node_75  = graph.emplace_back(MapNode(72, -276));
    auto& node_76  = graph.emplace_back(MapNode(72, -266));
    //Zona demoniaca (Pentagrama)
    auto& node_77  = graph.emplace_back(MapNode(128, -295));
    auto& node_78  = graph.emplace_back(MapNode(107, -311));
    auto& node_79  = graph.emplace_back(MapNode(129, -280));
    auto& node_80  = graph.emplace_back(MapNode(144, -299));
    auto& node_81  = graph.emplace_back(MapNode(127, -247));
    auto& node_82  = graph.emplace_back(MapNode(107, -233));
    auto& node_83  = graph.emplace_back(MapNode(129, -263));
    auto& node_84  = graph.emplace_back(MapNode(143, -245));
    auto& node_85  = graph.emplace_back(MapNode(175, -314));
    auto& node_86  = graph.emplace_back(MapNode(181, -333));
    auto& node_87  = graph.emplace_back(MapNode(159, -304));
    auto& node_88  = graph.emplace_back(MapNode(181, -297));
    auto& node_89  = graph.emplace_back(MapNode(203, -272));
    auto& node_90  = graph.emplace_back(MapNode(226, -272));
    auto& node_91  = graph.emplace_back(MapNode(190, -286));
    auto& node_92  = graph.emplace_back(MapNode(191, -260));
    auto& node_93  = graph.emplace_back(MapNode(175, -230));
    auto& node_94  = graph.emplace_back(MapNode(181, -209));
    auto& node_95  = graph.emplace_back(MapNode(182, -247));
    auto& node_96  = graph.emplace_back(MapNode(160, -240));
    //Demonios alrededor de estatua gigante de Lucyborg
    auto& node_97  = graph.emplace_back(MapNode(154, -292));
    auto& node_98  = graph.emplace_back(MapNode(180, -284));
    auto& node_99  = graph.emplace_back(MapNode(179, -259));
    auto& node_100  = graph.emplace_back(MapNode(154, -250));
    auto& node_101  = graph.emplace_back(MapNode(137, -271));
    //ZONA 4 -> Zona del caos (Zona final)
    auto& node_102  = graph.emplace_back(MapNode(-82, -484));
    auto& node_103  = graph.emplace_back(MapNode(-73, -479));
    auto& node_104  = graph.emplace_back(MapNode(-72, -473));
    auto& node_105  = graph.emplace_back(MapNode(-73, -462));
    auto& node_106  = graph.emplace_back(MapNode(-59, -461));
    auto& node_107  = graph.emplace_back(MapNode(-61, -473));
    auto& node_108  = graph.emplace_back(MapNode(-30, -494));
    auto& node_109  = graph.emplace_back(MapNode(-42, -495));
    auto& node_110  = graph.emplace_back(MapNode(-42, -506));
    auto& node_111  = graph.emplace_back(MapNode(-32, -506));
    auto& node_112  = graph.emplace_back(MapNode(-51, -507));
    auto& node_113  = graph.emplace_back(MapNode(-61, -506));
    auto& node_114  = graph.emplace_back(MapNode(-61, -494));
    auto& node_115  = graph.emplace_back(MapNode(-72, -494));
    auto& node_116  = graph.emplace_back(MapNode(-72, -506));
    auto& node_117  = graph.emplace_back(MapNode(-82, -504));
    auto& node_118  = graph.emplace_back(MapNode(-90, -497));
    auto& node_119  = graph.emplace_back(MapNode(-98, -493));
    auto& node_120  = graph.emplace_back(MapNode(-112, -507));
    auto& node_121  = graph.emplace_back(MapNode(-95, -518));
    auto& node_122  = graph.emplace_back(MapNode(-82, -531));
    auto& node_123  = graph.emplace_back(MapNode(-72, -527));
    auto& node_124  = graph.emplace_back(MapNode(-72, -538));
    auto& node_125  = graph.emplace_back(MapNode(-61, -538));
    auto& node_126  = graph.emplace_back(MapNode(-61, -527));
    auto& node_127  = graph.emplace_back(MapNode(-51, -528));
    auto& node_128  = graph.emplace_back(MapNode(-68, -518));
    auto& node_129  = graph.emplace_back(MapNode(-42, -526));
    auto& node_130  = graph.emplace_back(MapNode(-42, -538));
    auto& node_131  = graph.emplace_back(MapNode(-31, -526));
    auto& node_132  = graph.emplace_back(MapNode(-30, -539));
    auto& node_133  = graph.emplace_back(MapNode(-61, -569));
    auto& node_134  = graph.emplace_back(MapNode(-61, -552));
    auto& node_135  = graph.emplace_back(MapNode(-75, -554));
    auto& node_136  = graph.emplace_back(MapNode(-72, -570));
    auto& node_137  = graph.emplace_back(MapNode(-74, -576));
    auto& node_138  = graph.emplace_back(MapNode(-82, -573));
    auto& node_139  = graph.emplace_back(MapNode(-90, -576));
    auto& node_140  = graph.emplace_back(MapNode(-91, -570));
    auto& node_141  = graph.emplace_back(MapNode(-91, -558));
    auto& node_142  = graph.emplace_back(MapNode(-102, -558));
    auto& node_143  = graph.emplace_back(MapNode(-82, -548));
    auto& node_144  = graph.emplace_back(MapNode(-102, -570));
    auto& node_145  = graph.emplace_back(MapNode(-135, -539));
    auto& node_146  = graph.emplace_back(MapNode(-132, -526));
    auto& node_147  = graph.emplace_back(MapNode(-121, -538));
    auto& node_148  = graph.emplace_back(MapNode(-122, -526));
    auto& node_149  = graph.emplace_back(MapNode(-112, -528));
    auto& node_150  = graph.emplace_back(MapNode(-103, -526));
    auto& node_151  = graph.emplace_back(MapNode(-103, -539));
    auto& node_152  = graph.emplace_back(MapNode(-91, -539));
    auto& node_153  = graph.emplace_back(MapNode(-91, -526));
    auto& node_154  = graph.emplace_back(MapNode(-135, -494));
    auto& node_155  = graph.emplace_back(MapNode(-132, -506));
    auto& node_156  = graph.emplace_back(MapNode(-121, -506));
    auto& node_157  = graph.emplace_back(MapNode(-121, -494));
    auto& node_158  = graph.emplace_back(MapNode(-105, -461));
    auto& node_159  = graph.emplace_back(MapNode(-100, -472));
    auto& node_160  = graph.emplace_back(MapNode(-92, -463));
    auto& node_161  = graph.emplace_back(MapNode(-92, -472));
    auto& node_162  = graph.emplace_back(MapNode(-92, -479));

    //CONEXIONES ENTRE NODOS
    //ZONA 1 -> AMP. CITY
    node_0.connections.emplace_back(0, 1, length(node_0.coord - node_1.coord));

    node_1.connections.emplace_back(1, 0, length(node_1.coord - node_0.coord));
    node_1.connections.emplace_back(1, 2, length(node_1.coord - node_2.coord));

    node_2.connections.emplace_back(2, 1, length(node_2.coord - node_1.coord));
    node_2.connections.emplace_back(2, 4, length(node_2.coord - node_4.coord));

    node_3.connections.emplace_back(3, 2, length(node_3.coord - node_2.coord));
    node_3.connections.emplace_back(3, 4, length(node_3.coord - node_4.coord));
    node_3.connections.emplace_back(3, 6, length(node_3.coord - node_6.coord));

    node_4.connections.emplace_back(4, 3, length(node_4.coord - node_3.coord));
    node_4.connections.emplace_back(4, 5, length(node_4.coord - node_5.coord));
    node_4.connections.emplace_back(4, 6, length(node_4.coord - node_6.coord));

    node_5.connections.emplace_back(5, 4, length(node_5.coord - node_4.coord));
    node_5.connections.emplace_back(5, 6, length(node_5.coord - node_6.coord));

    node_6.connections.emplace_back(6, 3, length(node_6.coord - node_3.coord));
    node_6.connections.emplace_back(6, 4, length(node_6.coord - node_4.coord));
    node_6.connections.emplace_back(6, 5, length(node_6.coord - node_5.coord));
    node_6.connections.emplace_back(6, 7, length(node_6.coord - node_7.coord));

    node_7.connections.emplace_back(7, 6, length(node_7.coord - node_6.coord));
    node_7.connections.emplace_back(7, 8, length(node_7.coord - node_8.coord));
    node_7.connections.emplace_back(7, 9, length(node_7.coord - node_9.coord));

    //Puente 1 (Antes de abrir la puerta con las llaves)
    node_8.connections.emplace_back(8, 7, length(node_8.coord - node_7.coord));
    node_8.connections.emplace_back(8, 9, length(node_8.coord - node_9.coord));
    node_8.connections.emplace_back(8, 10, length(node_8.coord - node_10.coord));

    node_9.connections.emplace_back(9, 7, length(node_9.coord - node_7.coord));
    node_9.connections.emplace_back(9, 8, length(node_9.coord - node_8.coord));
    node_9.connections.emplace_back(9, 11, length(node_9.coord - node_11.coord));

    node_10.connections.emplace_back(10, 8, length(node_10.coord - node_8.coord));
    node_10.connections.emplace_back(10, 11, length(node_10.coord - node_11.coord));

    node_11.connections.emplace_back(11, 9, length(node_11.coord - node_9.coord));
    node_11.connections.emplace_back(11, 10, length(node_11.coord - node_10.coord));
    node_11.connections.emplace_back(11, 12, length(node_11.coord - node_12.coord));

    node_12.connections.emplace_back(12, 11, length(node_12.coord - node_11.coord));
    node_12.connections.emplace_back(12, 13, length(node_12.coord - node_13.coord));

    node_13.connections.emplace_back(13, 12, length(node_13.coord - node_12.coord));
    node_13.connections.emplace_back(13, 15, length(node_13.coord - node_15.coord));

    node_14.connections.emplace_back(14, 15, length(node_14.coord - node_15.coord));

    node_15.connections.emplace_back(15, 13, length(node_15.coord - node_13.coord));
    node_15.connections.emplace_back(15, 14, length(node_15.coord - node_14.coord));
    node_15.connections.emplace_back(15, 16, length(node_15.coord - node_16.coord));
    node_15.connections.emplace_back(15, 17, length(node_15.coord - node_17.coord));

    node_16.connections.emplace_back(16, 15, length(node_16.coord - node_15.coord));
    node_16.connections.emplace_back(16, 17, length(node_16.coord - node_17.coord));

    node_17.connections.emplace_back(17, 15, length(node_17.coord - node_15.coord));
    node_17.connections.emplace_back(17, 16, length(node_17.coord - node_16.coord));
    node_17.connections.emplace_back(17, 18, length(node_17.coord - node_18.coord));

    node_18.connections.emplace_back(18, 17, length(node_18.coord - node_17.coord));
    node_18.connections.emplace_back(18, 19, length(node_18.coord - node_19.coord));

    node_19.connections.emplace_back(19, 18, length(node_19.coord - node_18.coord));
    node_19.connections.emplace_back(19, 20, length(node_19.coord - node_20.coord));
    node_19.connections.emplace_back(19, 21, length(node_19.coord - node_21.coord));

    node_20.connections.emplace_back(20, 19, length(node_20.coord - node_19.coord));
    node_20.connections.emplace_back(20, 21, length(node_20.coord - node_21.coord));
    node_20.connections.emplace_back(20, 22, length(node_20.coord - node_22.coord));

    node_21.connections.emplace_back(21, 19, length(node_21.coord - node_19.coord));
    node_21.connections.emplace_back(21, 20, length(node_21.coord - node_20.coord));
    node_21.connections.emplace_back(21, 22, length(node_21.coord - node_22.coord));

    node_22.connections.emplace_back(22, 20, length(node_22.coord - node_20.coord));
    node_22.connections.emplace_back(22, 21, length(node_22.coord - node_21.coord));
    node_22.connections.emplace_back(22, 23, length(node_22.coord - node_23.coord));
    node_22.connections.emplace_back(22, 24, length(node_22.coord - node_24.coord));

    node_23.connections.emplace_back(23, 22, length(node_23.coord - node_22.coord));
    node_23.connections.emplace_back(23, 24, length(node_23.coord - node_24.coord));
    node_23.connections.emplace_back(23, 27, length(node_23.coord - node_27.coord));

    node_24.connections.emplace_back(24, 22, length(node_24.coord - node_22.coord));
    node_24.connections.emplace_back(24, 23, length(node_24.coord - node_23.coord));
    node_24.connections.emplace_back(24, 27, length(node_24.coord - node_27.coord));
    node_24.connections.emplace_back(24, 29, length(node_24.coord - node_29.coord));

    //Rellano entre zona Angelical y demoniaca (Justo despues del primer puente).
    node_25.connections.emplace_back(25, 26, length(node_25.coord - node_26.coord));
    node_25.connections.emplace_back(25, 28, length(node_25.coord - node_28.coord));

    node_26.connections.emplace_back(26, 25, length(node_26.coord - node_25.coord));
    node_26.connections.emplace_back(26, 27, length(node_26.coord - node_27.coord));
    node_26.connections.emplace_back(26, 28, length(node_26.coord - node_28.coord));

    node_27.connections.emplace_back(27, 23, length(node_27.coord - node_23.coord));
    node_27.connections.emplace_back(27, 24, length(node_27.coord - node_24.coord));
    node_27.connections.emplace_back(27, 26, length(node_27.coord - node_26.coord));
    node_27.connections.emplace_back(27, 28, length(node_27.coord - node_28.coord));
    node_27.connections.emplace_back(27, 29, length(node_27.coord - node_29.coord));
    node_27.connections.emplace_back(27, 31, length(node_27.coord - node_31.coord));

    node_28.connections.emplace_back(28, 25, length(node_28.coord - node_25.coord));
    node_28.connections.emplace_back(28, 26, length(node_28.coord - node_26.coord));
    node_28.connections.emplace_back(28, 27, length(node_28.coord - node_27.coord));
    node_28.connections.emplace_back(28, 30, length(node_28.coord - node_30.coord));
    node_28.connections.emplace_back(28, 33, length(node_28.coord - node_33.coord));

    node_29.connections.emplace_back(29, 24, length(node_29.coord - node_24.coord));
    node_29.connections.emplace_back(29, 27, length(node_29.coord - node_27.coord));
    node_29.connections.emplace_back(29, 32, length(node_29.coord - node_32.coord));
    node_29.connections.emplace_back(29, 60, length(node_29.coord - node_60.coord));

    node_30.connections.emplace_back(30, 28, length(node_30.coord - node_28.coord));
    node_30.connections.emplace_back(30, 31, length(node_30.coord - node_31.coord));

    node_31.connections.emplace_back(31, 27, length(node_31.coord - node_27.coord));
    node_31.connections.emplace_back(31, 30, length(node_31.coord - node_30.coord));
    node_31.connections.emplace_back(31, 32, length(node_31.coord - node_32.coord));

    node_32.connections.emplace_back(32, 29, length(node_32.coord - node_29.coord));
    node_32.connections.emplace_back(32, 31, length(node_32.coord - node_31.coord));
    node_32.connections.emplace_back(32, 102, length(node_32.coord - node_102.coord));

    //ZONA 2 -> CATEDRAL IGLESIA
    //Pasillo Principal
    node_33.connections.emplace_back(33, 28, length(node_33.coord - node_28.coord));
    node_33.connections.emplace_back(33, 34, length(node_33.coord - node_34.coord));
    node_33.connections.emplace_back(33, 35, length(node_33.coord - node_35.coord));

    node_34.connections.emplace_back(34, 33, length(node_34.coord - node_33.coord));
    node_34.connections.emplace_back(34, 35, length(node_34.coord - node_35.coord));

    node_35.connections.emplace_back(35, 33, length(node_35.coord - node_33.coord));
    node_35.connections.emplace_back(35, 34, length(node_35.coord - node_34.coord));
    node_35.connections.emplace_back(35, 36, length(node_35.coord - node_36.coord));

    node_36.connections.emplace_back(36, 35, length(node_36.coord - node_35.coord));
    node_36.connections.emplace_back(36, 37, length(node_36.coord - node_37.coord));
    node_36.connections.emplace_back(35, 39, length(node_36.coord - node_39.coord));

    node_37.connections.emplace_back(37, 36, length(node_37.coord - node_36.coord));
    node_37.connections.emplace_back(37, 38, length(node_37.coord - node_38.coord));

    node_38.connections.emplace_back(38, 37, length(node_38.coord - node_37.coord));

    node_39.connections.emplace_back(39, 36, length(node_39.coord - node_36.coord));
    node_39.connections.emplace_back(39, 40, length(node_39.coord - node_40.coord));
    node_39.connections.emplace_back(39, 41, length(node_39.coord - node_41.coord));

    node_40.connections.emplace_back(40, 39, length(node_40.coord - node_39.coord));

    node_41.connections.emplace_back(41, 39, length(node_41.coord - node_39.coord));
    node_41.connections.emplace_back(41, 42, length(node_41.coord - node_42.coord));
    node_41.connections.emplace_back(41, 43, length(node_41.coord - node_43.coord));
    node_41.connections.emplace_back(41, 44, length(node_41.coord - node_44.coord));

    node_42.connections.emplace_back(42, 41, length(node_42.coord - node_41.coord));

    node_43.connections.emplace_back(43, 41, length(node_43.coord - node_41.coord));
    node_43.connections.emplace_back(43, 47, length(node_43.coord - node_47.coord));
    node_43.connections.emplace_back(43, 49, length(node_43.coord - node_49.coord));

    node_44.connections.emplace_back(44, 41, length(node_44.coord - node_41.coord));
    node_44.connections.emplace_back(44, 46, length(node_44.coord - node_46.coord));
    node_44.connections.emplace_back(44, 54, length(node_44.coord - node_54.coord));

    node_45.connections.emplace_back(45, 46, length(node_45.coord - node_46.coord));

    node_46.connections.emplace_back(46, 44, length(node_46.coord - node_44.coord));
    node_46.connections.emplace_back(46, 45, length(node_46.coord - node_45.coord));
    node_46.connections.emplace_back(46, 59, length(node_46.coord - node_59.coord));

    node_47.connections.emplace_back(47, 43, length(node_47.coord - node_43.coord));
    node_47.connections.emplace_back(47, 48, length(node_47.coord - node_48.coord));
    node_47.connections.emplace_back(47, 57, length(node_47.coord - node_57.coord));

    node_48.connections.emplace_back(48, 47, length(node_48.coord - node_47.coord));

    //Zona Inferior (mesa con velas)
    node_49.connections.emplace_back(49, 43, length(node_49.coord - node_43.coord));
    node_49.connections.emplace_back(49, 50, length(node_49.coord - node_50.coord));

    node_50.connections.emplace_back(50, 49, length(node_50.coord - node_49.coord));
    node_50.connections.emplace_back(50, 51, length(node_50.coord - node_51.coord));
    node_50.connections.emplace_back(50, 52, length(node_50.coord - node_52.coord));

    node_51.connections.emplace_back(51, 50, length(node_51.coord - node_50.coord));

    node_52.connections.emplace_back(52, 50, length(node_52.coord - node_50.coord));

    //Zona Superior (Organo/Piano pa la musiquita jijiji)
    node_53.connections.emplace_back(53, 54, length(node_53.coord - node_54.coord));

    node_54.connections.emplace_back(54, 44, length(node_54.coord - node_44.coord));
    node_54.connections.emplace_back(54, 53, length(node_54.coord - node_53.coord));
    node_54.connections.emplace_back(54, 55, length(node_54.coord - node_55.coord));
    node_54.connections.emplace_back(54, 56, length(node_54.coord - node_56.coord));

    node_55.connections.emplace_back(55, 54, length(node_55.coord - node_54.coord));

    node_56.connections.emplace_back(56, 54, length(node_56.coord - node_54.coord));

    //Altar (Donde esta la llave)
    node_57.connections.emplace_back(57, 47, length(node_57.coord - node_47.coord));
    node_57.connections.emplace_back(57, 58, length(node_57.coord - node_58.coord));

    node_58.connections.emplace_back(58, 57, length(node_58.coord - node_57.coord));
    node_58.connections.emplace_back(58, 59, length(node_58.coord - node_59.coord));

    node_59.connections.emplace_back(59, 46, length(node_59.coord - node_46.coord));
    node_59.connections.emplace_back(59, 58, length(node_59.coord - node_58.coord));

    //ZONA 3 -> ZONA DE DEMONIOS (PASILLO + ZONA PENTAGRAMA)
    //Pasillo (Antes de llegar a la zona pentagrama)
    node_60.connections.emplace_back(60, 29, length(node_60.coord - node_29.coord));
    node_60.connections.emplace_back(60, 61, length(node_60.coord - node_61.coord));

    node_61.connections.emplace_back(61, 60, length(node_61.coord - node_60.coord));
    node_61.connections.emplace_back(61, 62, length(node_61.coord - node_62.coord));
    node_61.connections.emplace_back(61, 63, length(node_61.coord - node_63.coord));

    node_62.connections.emplace_back(62, 61, length(node_62.coord - node_61.coord));

    node_63.connections.emplace_back(63, 61, length(node_63.coord - node_61.coord));
    node_63.connections.emplace_back(63, 64, length(node_63.coord - node_64.coord));
    node_63.connections.emplace_back(63, 65, length(node_63.coord - node_65.coord));

    node_64.connections.emplace_back(64, 63, length(node_64.coord - node_63.coord));
    node_64.connections.emplace_back(64, 65, length(node_64.coord - node_65.coord));

    node_65.connections.emplace_back(65, 63, length(node_65.coord - node_63.coord));
    node_65.connections.emplace_back(65, 64, length(node_65.coord - node_64.coord));
    node_65.connections.emplace_back(65, 66, length(node_65.coord - node_66.coord));

    node_66.connections.emplace_back(66, 65, length(node_66.coord - node_65.coord));
    node_66.connections.emplace_back(66, 67, length(node_66.coord - node_67.coord));
    node_66.connections.emplace_back(66, 68, length(node_66.coord - node_68.coord));
    node_66.connections.emplace_back(66, 69, length(node_66.coord - node_69.coord));

    node_67.connections.emplace_back(67, 66, length(node_67.coord - node_66.coord));
    node_67.connections.emplace_back(67, 68, length(node_67.coord - node_68.coord));

    node_68.connections.emplace_back(68, 66, length(node_68.coord - node_66.coord));
    node_68.connections.emplace_back(68, 67, length(node_68.coord - node_67.coord));
    node_68.connections.emplace_back(68, 72, length(node_68.coord - node_72.coord));

    node_69.connections.emplace_back(69, 66, length(node_69.coord - node_66.coord));
    node_69.connections.emplace_back(69, 70, length(node_69.coord - node_70.coord));
    node_69.connections.emplace_back(69, 72, length(node_69.coord - node_72.coord));

    node_70.connections.emplace_back(70, 69, length(node_70.coord - node_69.coord));
    node_70.connections.emplace_back(70, 71, length(node_70.coord - node_71.coord));

    node_71.connections.emplace_back(71, 70, length(node_71.coord - node_70.coord));
    node_71.connections.emplace_back(71, 72, length(node_71.coord - node_72.coord));

    node_72.connections.emplace_back(72, 68, length(node_72.coord - node_68.coord));
    node_72.connections.emplace_back(72, 69, length(node_72.coord - node_69.coord));
    node_72.connections.emplace_back(72, 70, length(node_72.coord - node_70.coord));
    node_72.connections.emplace_back(72, 71, length(node_72.coord - node_71.coord));

    node_73.connections.emplace_back(73, 72, length(node_73.coord - node_72.coord));
    node_73.connections.emplace_back(73, 74, length(node_73.coord - node_74.coord));
    node_73.connections.emplace_back(73, 75, length(node_73.coord - node_75.coord));

    node_74.connections.emplace_back(74, 73, length(node_74.coord - node_73.coord));
    node_74.connections.emplace_back(74, 76, length(node_74.coord - node_76.coord));

    node_75.connections.emplace_back(75, 73, length(node_75.coord - node_73.coord));
    node_75.connections.emplace_back(75, 76, length(node_75.coord - node_76.coord));
    node_75.connections.emplace_back(75, 79, length(node_75.coord - node_79.coord));

    node_76.connections.emplace_back(76, 74, length(node_76.coord - node_74.coord));
    node_76.connections.emplace_back(76, 75, length(node_76.coord - node_75.coord));
    node_76.connections.emplace_back(76, 83, length(node_76.coord - node_83.coord));

    //Zona demoniaca (Pentagrama)
    node_77.connections.emplace_back(77, 78, length(node_77.coord - node_78.coord));
    node_77.connections.emplace_back(77, 79, length(node_77.coord - node_79.coord));
    node_77.connections.emplace_back(77, 80, length(node_77.coord - node_80.coord));

    node_78.connections.emplace_back(78, 77, length(node_78.coord - node_77.coord));

    node_79.connections.emplace_back(79, 75, length(node_79.coord - node_75.coord));
    node_79.connections.emplace_back(79, 77, length(node_79.coord - node_77.coord));
    node_79.connections.emplace_back(79, 101, length(node_79.coord - node_101.coord));

    node_80.connections.emplace_back(80, 79, length(node_80.coord - node_79.coord));
    node_80.connections.emplace_back(80, 97, length(node_80.coord - node_97.coord));

    node_81.connections.emplace_back(81, 82, length(node_81.coord - node_82.coord));
    node_81.connections.emplace_back(81, 83, length(node_81.coord - node_83.coord));
    node_81.connections.emplace_back(81, 84, length(node_81.coord - node_84.coord));

    node_82.connections.emplace_back(82, 81, length(node_82.coord - node_81.coord));

    node_83.connections.emplace_back(83, 76, length(node_83.coord - node_76.coord));
    node_83.connections.emplace_back(83, 81, length(node_83.coord - node_81.coord));
    node_83.connections.emplace_back(83, 101, length(node_83.coord - node_101.coord));

    node_84.connections.emplace_back(84, 81, length(node_84.coord - node_81.coord));
    node_84.connections.emplace_back(84, 100, length(node_84.coord - node_100.coord));

    node_85.connections.emplace_back(85, 86, length(node_85.coord - node_86.coord));
    node_85.connections.emplace_back(85, 87, length(node_85.coord - node_87.coord));
    node_85.connections.emplace_back(85, 88, length(node_85.coord - node_88.coord));

    node_86.connections.emplace_back(86, 85, length(node_86.coord - node_85.coord));

    node_87.connections.emplace_back(87, 85, length(node_87.coord - node_85.coord));
    node_87.connections.emplace_back(87, 97, length(node_87.coord - node_97.coord));

    node_88.connections.emplace_back(88, 85, length(node_88.coord - node_85.coord));
    node_88.connections.emplace_back(88, 98, length(node_88.coord - node_98.coord));

    node_89.connections.emplace_back(89, 90, length(node_89.coord - node_90.coord));
    node_89.connections.emplace_back(89, 91, length(node_89.coord - node_91.coord));
    node_89.connections.emplace_back(89, 92, length(node_89.coord - node_92.coord));

    node_90.connections.emplace_back(90, 89, length(node_90.coord - node_89.coord));

    node_91.connections.emplace_back(91, 89, length(node_91.coord - node_89.coord));
    node_91.connections.emplace_back(91, 98, length(node_91.coord - node_98.coord));

    node_92.connections.emplace_back(92, 89, length(node_92.coord - node_89.coord));
    node_92.connections.emplace_back(92, 99, length(node_92.coord - node_99.coord));

    node_93.connections.emplace_back(93, 94, length(node_93.coord - node_94.coord));
    node_93.connections.emplace_back(93, 95, length(node_93.coord - node_95.coord));
    node_93.connections.emplace_back(93, 96, length(node_93.coord - node_96.coord));

    node_94.connections.emplace_back(94, 93, length(node_94.coord - node_93.coord));

    node_95.connections.emplace_back(95, 93, length(node_95.coord - node_93.coord));
    node_95.connections.emplace_back(95, 99, length(node_95.coord - node_99.coord));

    node_96.connections.emplace_back(96, 93, length(node_96.coord - node_93.coord));
    node_96.connections.emplace_back(96, 100, length(node_96.coord - node_100.coord));

    //Demonios alrededor de estatua gigante de Lucyborg
    node_97.connections.emplace_back(97, 80, length(node_97.coord - node_80.coord));
    node_97.connections.emplace_back(97, 87, length(node_97.coord - node_87.coord));
    node_97.connections.emplace_back(97, 98, length(node_97.coord - node_98.coord));
    node_97.connections.emplace_back(97, 101, length(node_97.coord - node_101.coord));

    node_98.connections.emplace_back(98, 88, length(node_98.coord - node_88.coord));
    node_98.connections.emplace_back(98, 91, length(node_98.coord - node_91.coord));
    node_98.connections.emplace_back(98, 97, length(node_98.coord - node_97.coord));
    node_98.connections.emplace_back(98, 99, length(node_98.coord - node_99.coord));

    node_99.connections.emplace_back(99, 92, length(node_99.coord - node_92.coord));
    node_99.connections.emplace_back(99, 95, length(node_99.coord - node_95.coord));
    node_99.connections.emplace_back(99, 98, length(node_99.coord - node_98.coord));
    node_99.connections.emplace_back(99, 100, length(node_99.coord - node_100.coord));

    node_100.connections.emplace_back(100, 84, length(node_100.coord - node_84.coord));
    node_100.connections.emplace_back(100, 96, length(node_100.coord - node_96.coord));
    node_100.connections.emplace_back(100, 99, length(node_100.coord - node_99.coord));
    node_100.connections.emplace_back(100, 101, length(node_100.coord - node_101.coord));

    node_101.connections.emplace_back(101, 79, length(node_101.coord - node_79.coord));
    node_101.connections.emplace_back(101, 83, length(node_101.coord - node_83.coord));
    node_101.connections.emplace_back(101, 97, length(node_101.coord - node_97.coord));
    node_101.connections.emplace_back(101, 100, length(node_101.coord - node_100.coord));

    //ZONA 4 -> Zona del caos (Zona final)
    node_102.connections.emplace_back(102, 32, length(node_102.coord - node_32.coord));
    node_102.connections.emplace_back(102, 103, length(node_102.coord - node_103.coord));
    node_102.connections.emplace_back(102, 117, length(node_102.coord - node_117.coord));
    node_102.connections.emplace_back(102, 162, length(node_102.coord - node_162.coord));

    node_103.connections.emplace_back(103, 102, length(node_103.coord - node_102.coord));
    node_103.connections.emplace_back(103, 104, length(node_103.coord - node_104.coord));

    node_104.connections.emplace_back(104, 103, length(node_104.coord - node_103.coord));
    node_104.connections.emplace_back(104, 105, length(node_104.coord - node_105.coord));
    node_104.connections.emplace_back(104, 107, length(node_104.coord - node_107.coord));

    node_105.connections.emplace_back(105, 104, length(node_105.coord - node_104.coord));
    node_105.connections.emplace_back(105, 106, length(node_105.coord - node_106.coord));

    node_106.connections.emplace_back(106, 105, length(node_106.coord - node_105.coord));
    node_106.connections.emplace_back(106, 107, length(node_106.coord - node_107.coord));
    node_106.connections.emplace_back(106, 108, length(node_106.coord - node_108.coord));

    node_107.connections.emplace_back(107, 104, length(node_107.coord - node_104.coord));
    node_107.connections.emplace_back(107, 106, length(node_107.coord - node_106.coord));

    node_108.connections.emplace_back(108, 106, length(node_108.coord - node_106.coord));
    node_108.connections.emplace_back(108, 109, length(node_108.coord - node_109.coord));
    node_108.connections.emplace_back(108, 111, length(node_108.coord - node_111.coord));
    node_108.connections.emplace_back(108, 132, length(node_108.coord - node_132.coord));

    node_109.connections.emplace_back(109, 108, length(node_109.coord - node_108.coord));
    node_109.connections.emplace_back(109, 110, length(node_109.coord - node_110.coord));

    node_110.connections.emplace_back(110, 109, length(node_110.coord - node_109.coord));
    node_110.connections.emplace_back(110, 111, length(node_110.coord - node_111.coord));
    node_110.connections.emplace_back(110, 112, length(node_110.coord - node_112.coord));

    node_111.connections.emplace_back(111, 108, length(node_111.coord - node_108.coord));
    node_111.connections.emplace_back(111, 110, length(node_111.coord - node_110.coord));

    node_112.connections.emplace_back(112, 110, length(node_112.coord - node_110.coord));
    node_112.connections.emplace_back(112, 113, length(node_112.coord - node_113.coord));
    node_112.connections.emplace_back(112, 128, length(node_112.coord - node_128.coord));

    node_113.connections.emplace_back(113, 112, length(node_113.coord - node_116.coord));
    node_113.connections.emplace_back(113, 114, length(node_113.coord - node_114.coord));
    node_113.connections.emplace_back(113, 116, length(node_113.coord - node_116.coord));

    node_114.connections.emplace_back(114, 113, length(node_114.coord - node_113.coord));
    node_114.connections.emplace_back(114, 115, length(node_114.coord - node_115.coord));

    node_115.connections.emplace_back(115, 114, length(node_115.coord - node_114.coord));
    node_115.connections.emplace_back(115, 116, length(node_115.coord - node_116.coord));

    node_116.connections.emplace_back(116, 113, length(node_116.coord - node_113.coord));
    node_116.connections.emplace_back(116, 115, length(node_116.coord - node_115.coord));
    node_116.connections.emplace_back(116, 117, length(node_116.coord - node_117.coord));

    node_117.connections.emplace_back(117, 102, length(node_117.coord - node_102.coord));
    node_117.connections.emplace_back(117, 116, length(node_117.coord - node_116.coord));
    node_117.connections.emplace_back(117, 118, length(node_117.coord - node_118.coord));
    node_117.connections.emplace_back(117, 121, length(node_117.coord - node_121.coord));
    node_117.connections.emplace_back(117, 128, length(node_117.coord - node_128.coord));

    node_118.connections.emplace_back(118, 117, length(node_118.coord - node_117.coord));
    node_118.connections.emplace_back(118, 119, length(node_118.coord - node_119.coord));
    node_118.connections.emplace_back(118, 121, length(node_118.coord - node_121.coord));

    node_119.connections.emplace_back(119, 118, length(node_119.coord - node_118.coord));
    node_119.connections.emplace_back(119, 120, length(node_119.coord - node_120.coord));

    node_120.connections.emplace_back(120, 119, length(node_120.coord - node_119.coord));
    node_120.connections.emplace_back(120, 121, length(node_120.coord - node_121.coord));
    node_120.connections.emplace_back(120, 156, length(node_120.coord - node_156.coord));

    node_121.connections.emplace_back(121, 117, length(node_121.coord - node_117.coord));
    node_121.connections.emplace_back(121, 118, length(node_121.coord - node_118.coord));
    node_121.connections.emplace_back(121, 120, length(node_121.coord - node_120.coord));
    node_121.connections.emplace_back(121, 122, length(node_121.coord - node_122.coord));
    node_121.connections.emplace_back(121, 149, length(node_121.coord - node_149.coord));

    node_122.connections.emplace_back(122, 121, length(node_122.coord - node_121.coord));
    node_122.connections.emplace_back(122, 123, length(node_122.coord - node_123.coord));
    node_122.connections.emplace_back(122, 128, length(node_122.coord - node_128.coord));
    node_122.connections.emplace_back(122, 143, length(node_122.coord - node_143.coord));
    node_122.connections.emplace_back(122, 153, length(node_122.coord - node_153.coord));

    node_123.connections.emplace_back(123, 122, length(node_123.coord - node_122.coord));
    node_123.connections.emplace_back(123, 124, length(node_123.coord - node_124.coord));
    node_123.connections.emplace_back(123, 126, length(node_123.coord - node_126.coord));

    node_124.connections.emplace_back(124, 123, length(node_124.coord - node_123.coord));
    node_124.connections.emplace_back(124, 125, length(node_124.coord - node_125.coord));

    node_125.connections.emplace_back(125, 124, length(node_125.coord - node_124.coord));
    node_125.connections.emplace_back(125, 126, length(node_125.coord - node_126.coord));

    node_126.connections.emplace_back(126, 123, length(node_126.coord - node_123.coord));
    node_126.connections.emplace_back(126, 125, length(node_126.coord - node_125.coord));
    node_126.connections.emplace_back(126, 127, length(node_126.coord - node_127.coord));

    node_127.connections.emplace_back(127, 126, length(node_127.coord - node_126.coord));
    node_127.connections.emplace_back(127, 128, length(node_127.coord - node_128.coord));
    node_127.connections.emplace_back(127, 129, length(node_127.coord - node_129.coord));

    node_128.connections.emplace_back(128, 112, length(node_128.coord - node_112.coord));
    node_128.connections.emplace_back(128, 117, length(node_128.coord - node_117.coord));
    node_128.connections.emplace_back(128, 122, length(node_128.coord - node_122.coord));
    node_128.connections.emplace_back(128, 127, length(node_128.coord - node_127.coord));

    node_129.connections.emplace_back(129, 127, length(node_129.coord - node_127.coord));
    node_129.connections.emplace_back(129, 130, length(node_129.coord - node_130.coord));
    node_129.connections.emplace_back(129, 131, length(node_129.coord - node_131.coord));

    node_130.connections.emplace_back(130, 129, length(node_130.coord - node_129.coord));
    node_130.connections.emplace_back(130, 132, length(node_130.coord - node_132.coord));

    node_131.connections.emplace_back(131, 129, length(node_131.coord - node_129.coord));
    node_131.connections.emplace_back(131, 132, length(node_131.coord - node_132.coord));

    node_132.connections.emplace_back(132, 108, length(node_132.coord - node_108.coord));
    node_132.connections.emplace_back(132, 130, length(node_132.coord - node_130.coord));
    node_132.connections.emplace_back(132, 131, length(node_132.coord - node_131.coord));
    node_132.connections.emplace_back(132, 133, length(node_132.coord - node_133.coord));

    node_133.connections.emplace_back(133, 132, length(node_133.coord - node_132.coord));
    node_133.connections.emplace_back(133, 134, length(node_133.coord - node_134.coord));
    node_133.connections.emplace_back(133, 136, length(node_133.coord - node_136.coord));

    node_134.connections.emplace_back(134, 133, length(node_134.coord - node_133.coord));
    node_134.connections.emplace_back(134, 135, length(node_134.coord - node_135.coord));

    node_135.connections.emplace_back(135, 134, length(node_135.coord - node_134.coord));
    node_135.connections.emplace_back(135, 136, length(node_135.coord - node_136.coord));
    node_135.connections.emplace_back(135, 143, length(node_135.coord - node_143.coord));

    node_136.connections.emplace_back(136, 133, length(node_136.coord - node_133.coord));
    node_136.connections.emplace_back(136, 135, length(node_136.coord - node_135.coord));
    node_136.connections.emplace_back(136, 137, length(node_136.coord - node_137.coord));

    node_137.connections.emplace_back(137, 136, length(node_137.coord - node_136.coord));
    node_137.connections.emplace_back(137, 138, length(node_137.coord - node_138.coord));

    node_138.connections.emplace_back(138, 137, length(node_138.coord - node_137.coord));
    node_138.connections.emplace_back(138, 139, length(node_138.coord - node_139.coord));
    node_138.connections.emplace_back(138, 143, length(node_138.coord - node_143.coord));

    node_139.connections.emplace_back(139, 138, length(node_139.coord - node_138.coord));
    node_139.connections.emplace_back(139, 140, length(node_139.coord - node_140.coord));

    node_140.connections.emplace_back(140, 139, length(node_140.coord - node_139.coord));
    node_140.connections.emplace_back(140, 141, length(node_140.coord - node_141.coord));
    node_140.connections.emplace_back(140, 144, length(node_140.coord - node_144.coord));

    node_141.connections.emplace_back(141, 140, length(node_141.coord - node_140.coord));
    node_141.connections.emplace_back(141, 142, length(node_141.coord - node_142.coord));
    node_141.connections.emplace_back(141, 143, length(node_141.coord - node_143.coord));

    node_142.connections.emplace_back(142, 141, length(node_142.coord - node_141.coord));
    node_142.connections.emplace_back(142, 144, length(node_142.coord - node_144.coord));

    node_143.connections.emplace_back(143, 122, length(node_143.coord - node_122.coord));
    node_143.connections.emplace_back(143, 135, length(node_143.coord - node_135.coord));
    node_143.connections.emplace_back(143, 138, length(node_143.coord - node_138.coord));
    node_143.connections.emplace_back(143, 141, length(node_143.coord - node_141.coord));

    node_144.connections.emplace_back(144, 140, length(node_144.coord - node_140.coord));
    node_144.connections.emplace_back(144, 142, length(node_144.coord - node_142.coord));
    node_144.connections.emplace_back(144, 145, length(node_144.coord - node_145.coord));

    node_145.connections.emplace_back(145, 144, length(node_145.coord - node_144.coord));
    node_145.connections.emplace_back(145, 146, length(node_145.coord - node_146.coord));
    node_145.connections.emplace_back(145, 147, length(node_145.coord - node_147.coord));
    node_145.connections.emplace_back(145, 154, length(node_145.coord - node_154.coord));

    node_146.connections.emplace_back(146, 145, length(node_146.coord - node_145.coord));
    node_146.connections.emplace_back(146, 148, length(node_146.coord - node_148.coord));

    node_147.connections.emplace_back(147, 145, length(node_147.coord - node_145.coord));
    node_147.connections.emplace_back(147, 148, length(node_147.coord - node_148.coord));

    node_148.connections.emplace_back(148, 146, length(node_148.coord - node_146.coord));
    node_148.connections.emplace_back(148, 147, length(node_148.coord - node_147.coord));
    node_148.connections.emplace_back(148, 149, length(node_148.coord - node_149.coord));

    node_149.connections.emplace_back(149, 121, length(node_149.coord - node_121.coord));
    node_149.connections.emplace_back(149, 148, length(node_149.coord - node_148.coord));
    node_149.connections.emplace_back(149, 150, length(node_149.coord - node_150.coord));

    node_150.connections.emplace_back(150, 149, length(node_150.coord - node_149.coord));
    node_150.connections.emplace_back(150, 151, length(node_150.coord - node_151.coord));
    node_150.connections.emplace_back(150, 153, length(node_150.coord - node_153.coord));

    node_151.connections.emplace_back(151, 150, length(node_151.coord - node_150.coord));
    node_151.connections.emplace_back(151, 152, length(node_151.coord - node_152.coord));

    node_152.connections.emplace_back(152, 151, length(node_152.coord - node_151.coord));
    node_152.connections.emplace_back(152, 153, length(node_152.coord - node_153.coord));

    node_153.connections.emplace_back(153, 122, length(node_153.coord - node_122.coord));
    node_153.connections.emplace_back(153, 150, length(node_153.coord - node_150.coord));
    node_153.connections.emplace_back(153, 152, length(node_153.coord - node_152.coord));

    node_154.connections.emplace_back(154, 145, length(node_154.coord - node_145.coord));
    node_154.connections.emplace_back(154, 155, length(node_154.coord - node_155.coord));
    node_154.connections.emplace_back(154, 157, length(node_154.coord - node_157.coord));
    node_154.connections.emplace_back(154, 158, length(node_154.coord - node_158.coord));

    node_155.connections.emplace_back(155, 154, length(node_155.coord - node_154.coord));
    node_155.connections.emplace_back(155, 156, length(node_155.coord - node_156.coord));

    node_156.connections.emplace_back(156, 120, length(node_156.coord - node_120.coord));
    node_156.connections.emplace_back(156, 155, length(node_156.coord - node_155.coord));
    node_156.connections.emplace_back(156, 157, length(node_156.coord - node_157.coord));

    node_157.connections.emplace_back(157, 154, length(node_157.coord - node_154.coord));
    node_157.connections.emplace_back(157, 156, length(node_157.coord - node_156.coord));

    node_158.connections.emplace_back(158, 154, length(node_158.coord - node_154.coord));
    node_158.connections.emplace_back(158, 159, length(node_158.coord - node_159.coord));
    node_158.connections.emplace_back(158, 160, length(node_158.coord - node_160.coord));

    node_159.connections.emplace_back(159, 158, length(node_159.coord - node_158.coord));
    node_159.connections.emplace_back(159, 161, length(node_159.coord - node_161.coord));

    node_160.connections.emplace_back(160, 158, length(node_160.coord - node_158.coord));
    node_160.connections.emplace_back(160, 161, length(node_160.coord - node_161.coord));

    node_161.connections.emplace_back(161, 159, length(node_161.coord - node_159.coord));
    node_161.connections.emplace_back(161, 160, length(node_161.coord - node_160.coord));
    node_161.connections.emplace_back(161, 162, length(node_161.coord - node_162.coord));

    node_162.connections.emplace_back(162, 102, length(node_162.coord - node_102.coord));
    node_162.connections.emplace_back(162, 161, length(node_162.coord - node_161.coord));

}

bool EntityManager::checkVictory() const {
	//return enemiesLeft <= 0;
	return player->getComponent<Physics>()->position.z < -596;
}

bool EntityManager::checkDefeat() const {
	return !greater_e(player->getComponent<CharacterData>()->health, 0.f);
}

void EntityManager::createIntro() {
	initData(8, 0, 8);

	createMenuOptions(0, 4);

	createVideo("../resources/videos/intro/1_F.mp4", false, VIDEO_INTRO_GAME);
	createVideo("../resources/videos/intro/2_F_L.mp4", true, NO_SOUND);

	createTexture("../resources/menu/main_menu/op_menu_1.png", 0, 0);
	createTexture("../resources/menu/main_menu/op_menu_2.png", 0, 0);
	createTexture("../resources/menu/main_menu/op_menu_3.png", 0, 0);
	createTexture("../resources/menu/main_menu/op_menu_4.png", 0, 0);

	componentStorage.printComponentStorage();
}

void EntityManager::createTutorial() {
	cleanData();

	initData(8, 0, 8);

	createVideo("../resources/videos/tutorial/1_F.mp4", false, VIDEO_TUTORIAL);

	componentStorage.printComponentStorage();
}

void EntityManager::nextVideo() {
	idxVideo++;
}