#include <cmp/Storage.hpp>

void Storage::initData(const int maxComponents) {
	std::get<vector<AI>>(map[AI_TYPE] = std::move(vector<AI>())).reserve(maxComponents);
	std::get<vector<std::unique_ptr<INode>>>(map[INODE_TYPE] = std::move(vector<std::unique_ptr<INode>>())).reserve(maxComponents);
	std::get<vector<Transformable>>(map[TRANSFORMABLE_TYPE] = std::move(vector<Transformable>())).reserve(maxComponents);
	std::get<vector<Velocity>>(map[VELOCITY_TYPE] = std::move(vector<Velocity>())).reserve(maxComponents);
	std::get<vector<BoundingBox>>(map[STATIC_BOUNDING_BOX_TYPE] = std::move(vector<BoundingBox>())).reserve(maxComponents);
	std::get<vector<BoundingBox>>(map[SPECIAL_BOUNDING_BOX_TYPE] = std::move(vector<BoundingBox>())).reserve(maxComponents);
	std::get<vector<BoundingBox>>(map[RAY_BOUNDING_BOX_TYPE] = std::move(vector<BoundingBox>())).reserve(maxComponents);
	std::get<vector<Physics>>(map[PHYSICS_TYPE] = std::move(vector<Physics>())).reserve(maxComponents);
	std::get<vector<BulletData>>(map[BULLET_DATA_TYPE] = std::move(vector<BulletData>())).reserve(maxComponents);
	std::get<vector<CharacterData>>(map[CHARACTER_DATA_TYPE] = std::move(vector<CharacterData>())).reserve(maxComponents);
}

void Storage::cleanData() {
	Component::resetIDManagementValue();	// ID de los componentes a 0

	map.clear();							// limpiamos todos los vectores
}

Storage::~Storage() = default;

const variantComponentVectorTypes& Storage::getComponents(const ComponentType type) const {
	return const_cast<std::unordered_map<ComponentType,variantComponentVectorTypes>&>(map)[type];
}

variantComponentVectorTypes& Storage::getComponents(const ComponentType type) {
	return const_cast<variantComponentVectorTypes&>(std::as_const(*this).getComponents(type));
}