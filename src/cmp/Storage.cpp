#include <cmp/Storage.hpp>
#include <SunlightEngine/SceneNode.hpp>
#include <SunlightEngine/CameraNode.hpp>

using Sun::SceneNode;
using Sun::CameraNode;

// TODO : VALORES RESERVADOS DE LOS VECTORES APROXIMADOS (hay margen, pero tener cuidado)

Storage::Storage(std::size_t initialSize) {
	std::get<vector<AI>>(map[AI_TYPE] = std::move(vector<AI>())).reserve(16);
	std::get<vector<Node_ptr>>(map[NODE_TYPE] = std::move(vector<Node_ptr>())).reserve(128);
	std::get<vector<Transformable>>(map[TRANSFORMABLE_TYPE] = std::move(vector<Transformable>())).reserve(64);
	std::get<vector<Velocity>>(map[VELOCITY_TYPE] = std::move(vector<Velocity>())).reserve(32);
	std::get<vector<BoundingBox>>(map[STATIC_BOUNDING_BOX_TYPE] = std::move(vector<BoundingBox>())).reserve(64);
	std::get<vector<BoundingBox>>(map[SPECIAL_BOUNDING_BOX_TYPE] = std::move(vector<BoundingBox>())).reserve(32);
	std::get<vector<BoundingBox>>(map[RAY_BOUNDING_BOX_TYPE] = std::move(vector<BoundingBox>())).reserve(16);
	std::get<vector<Physics>>(map[PHYSICS_TYPE] = std::move(vector<Physics>())).reserve(128);
	std::get<vector<BulletData>>(map[BULLET_DATA_TYPE] = std::move(vector<BulletData>())).reserve(16);
	std::get<vector<CharacterData>>(map[CHARACTER_DATA_TYPE] = std::move(vector<CharacterData>())).reserve(16);
}

Storage::~Storage() {
	map.clear();
}

Node_ptr& Storage::createSceneNode(const Device& device, const Vector3f& position, const Vector3f& rotation, const Vector3f& dim, const char* mesh, const char* texture) {
	std::cout 	<< "Capacity: " << std::get<vector<Node_ptr>>(map[NODE_TYPE]).capacity() 	<< std::endl
				<< "Size: " 	<< std::get<vector<Node_ptr>>(map[NODE_TYPE]).size() 		<< std::endl;

	for (auto& cmp : std::get<vector<Node_ptr>>(map[NODE_TYPE]))
		if (!(*cmp))
			return *(new (&cmp) Node_ptr(std::make_unique<SceneNode>(device, position, rotation, dim, mesh, texture)));

	return std::get<vector<Node_ptr>>(map[NODE_TYPE]).emplace_back(std::make_unique<SceneNode>(device, position, rotation, dim, mesh, texture));
}

Node_ptr& Storage::createCameraNode(const Device& device, const Vector3f& position, const Vector3f& target) {
	std::cout 	<< "Capacity: " << std::get<vector<Node_ptr>>(map[NODE_TYPE]).capacity() 	<< std::endl
				 << "Size: " 	<< std::get<vector<Node_ptr>>(map[NODE_TYPE]).size() 		<< std::endl;

	for (auto& cmp : std::get<vector<Node_ptr>>(map[NODE_TYPE]))
		if(!(*cmp))
			return *(new (&cmp) Node_ptr(std::make_unique<CameraNode>(device, target, position)));

	return std::get<vector<Node_ptr>>(map[NODE_TYPE]).emplace_back(std::make_unique<CameraNode>(device, target, position));
}

const variantComponentVectorTypes& Storage::getComponents(const ComponentType type) const {
	return const_cast<std::unordered_map<ComponentType,variantComponentVectorTypes>&>(map)[type];
}

variantComponentVectorTypes& Storage::getComponents(const ComponentType type) {
	return const_cast<variantComponentVectorTypes&>(std::as_const(*this).getComponents(type));
}