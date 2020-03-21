#include <cmp/Storage.hpp>

void Storage::initData(const unsigned maxComponents) {
/*
	initVector<AI>(5);
	initVector<Transformable>(59);
	initVector<Velocity>(8);
	initVector<Physics>(9);
	initVector<BulletData>(2);
	initVector<CharacterData>(8);
	initVector<TriggerMovSphere>(6);
	initVector<TriggerStaticAABB>(16);
	initVector<RigidStaticAABB>(49);
	initVector<TriggerFastMov>(2);
	initVector<Graph>(1);
	initVector<Render>(68);
*/
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

	engine.scene.get();

	if (!returnValue)
		returnValue = nodes.emplace_back(engine.scene->addMeshNode(mesh));

	return returnValue;
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

INode * Storage::createLight(float amb, float diff, float spe) {

	INode * returnValue { nullptr };

	for (auto & node : nodes) {
		if (node == nullptr) {
			returnValue = node = engine.scene->addFreeLightNode(amb, diff, spe);
			break;
		}
	}

	if (!returnValue)
		returnValue = nodes.emplace_back(engine.scene->addFreeLightNode(amb, diff, spe));

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
