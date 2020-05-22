#include <cmp/Storage.hpp>

void Storage::initData(const unsigned maxComponents) {
	initVector<AI>(maxComponents);
	initVector<Transformable>(maxComponents);
	initVector<Velocity>(maxComponents);
	initVector<Physics>(maxComponents);
	initVector<BulletData>(maxComponents);
	initVector<CharacterData>(maxComponents);
	initVector<TriggerMovSphere>(maxComponents);
	initVector<TriggerStaticAABB>(maxComponents);
	initVector<RigidStaticAABB>(maxComponents);
	initVector<TriggerFastMov>(maxComponents);
	initVector<Graph>(maxComponents);
	initVector<Render>(maxComponents);
	initVector<Jump>(maxComponents); //crear los justos para demons
	initVector<Video>(maxComponents);
	initVector<CircleBounding>(maxComponents);
	initVector<TriangleOBB>(256); // (son 239, pero soy un chulo y pongo 256 porque es potencia de 2 jaja no se que coño hago xD)
	initVector<TextureCmp>(maxComponents);
	initVector<MenuOption>(1);
    initVector<BackgroundMusic>(1);

	nodes.reserve(maxComponents);
}

void Storage::cleanData() {
	Component::resetIDManagementValue();	// ID de los componentes a 0

	for (const auto & node : nodes)
		if (node)
			node->remove();

	nodes.clear();

	map.clear();
}

INode * Storage::createMesh(const std::string_view mesh) {

//		std::cout << "\n\n" << "Node" << "\n";
//		printVecInfo(nodes);

	INode * returnValue { nullptr };

	for (auto & node : nodes) {
		if (node == nullptr) {
			returnValue = node = engine.scene->addMeshNode(mesh);
			break;
		}
	}

	if (!returnValue)
		returnValue = nodes.emplace_back(engine.scene->addMeshNode(mesh));

	return returnValue;
}

void Storage::setMesh(INode* node, const std::string_view mesh){
	engine.scene->setMesh(node, mesh);
}

INode * Storage::createAnimatedMesh(const std::string_view path) {

//		std::cout << "\n\n" << "Node" << "\n";
//		printVecInfo(nodes);

	INode * returnValue { nullptr };

	for (auto & node : nodes) {
		if (node == nullptr) {
			returnValue = node = engine.scene->addAnimatedMeshNode(path);
			break;
		}
	}

	if (!returnValue)
		returnValue = nodes.emplace_back(engine.scene->addAnimatedMeshNode(path));

	return returnValue;
}

void Storage::setAnimatedMesh(INode* node, const std::string_view animation){
	engine.scene->setAnimatedMesh(node, animation);
}

INode * Storage::createCamera() {

//		std::cout << "\n\n" << "Node" << "\n";
//		printVecInfo(nodes);

	INode * returnValue { nullptr };

	for (auto & node : nodes) {
		if (node == nullptr) {
			returnValue = node = engine.scene->addFreeCameraNode();
			break;
		}
	}

	if (!returnValue)
		returnValue = nodes.emplace_back(engine.scene->addFreeCameraNode());

	return returnValue;
}

INode * Storage::createLight(const glm::vec3& diff, const glm::vec3& spe, const glm::vec3& dir) {
	INode * returnValue { nullptr };

	for (auto & node : nodes) {
		if (node == nullptr) {
			returnValue = node = engine.scene->addFreeLightNode(diff, spe, dir);
			break;
		}
	}
	
	if (!returnValue)
		returnValue = nodes.emplace_back(engine.scene->addFreeLightNode(diff, spe, dir));

	return returnValue;
}

void Storage::removeNode(const INode * n) {
	for (auto & node : nodes) {
		if (n == node) {
			node = nullptr;
			return;
		}
	}
}

void Storage::printComponentStorage() const {
	printVecInfo<AI>();
	printVecInfo<Transformable>();
	printVecInfo<Velocity>();
	printVecInfo<Physics>();
	printVecInfo<BulletData>();
	printVecInfo<CharacterData>();
	printVecInfo<TriggerMovSphere>();
	printVecInfo<TriggerStaticAABB>();
	printVecInfo<RigidStaticAABB>();
	printVecInfo<TriggerFastMov>();
	printVecInfo<Graph>();
	printVecInfo<Render>();
	printVecInfo<Jump>();
	printVecInfo<Video>();
	printVecInfo<CircleBounding>();
	printVecInfo<TriangleOBB>();
	printVecInfo<TextureCmp>();
	printVecInfo<MenuOption>();
    printVecInfo<BackgroundMusic>();
}
