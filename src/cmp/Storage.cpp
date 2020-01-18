#include <cmp/Storage.hpp>
#include <SunlightEngine/SceneNode.hpp>
#include <SunlightEngine/CameraNode.hpp>
#include <new>

// TODO : REFACTORIZAR CODIGO usando "variant" para los distintos tipos de componentes como se ha hecho con los distintos tipos de vectores

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

/*
	spacesAvailable[AI_TYPE].reserve(32);
	spacesAvailable[NODE_TYPE].reserve(32);
	spacesAvailable[TRANSFORMABLE_TYPE].reserve(32);
	spacesAvailable[VELOCITY_TYPE].reserve(32);
	spacesAvailable[STATIC_BOUNDING_BOX_TYPE].reserve(32);
	spacesAvailable[SPECIAL_BOUNDING_BOX_TYPE].reserve(32);
	spacesAvailable[RAY_BOUNDING_BOX_TYPE].reserve(32);
	spacesAvailable[PHYSICS_TYPE].reserve(32);
	spacesAvailable[BULLET_DATA_TYPE].reserve(32);
	spacesAvailable[CHARACTER_DATA_TYPE].reserve(32);
*/
}

Storage::~Storage() {
	map.clear();
//	spacesAvailable.clear();
}

AI& Storage::createAI(const EntityType e_type, const std::size_t e_ID, const vector<Vector3f>& patrol) {
	std::cout 	<< "Capacity: " << std::get<vector<AI>>(map[AI_TYPE]).capacity() 	<< std::endl
				<< "Size: " 	<< std::get<vector<AI>>(map[AI_TYPE]).size() 		<< std::endl;

	return createComponent(std::get<vector<AI>>(map[AI_TYPE]), e_type, e_ID, patrol);
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

Transformable& Storage::createTransformable(const EntityType e_type, const std::size_t e_ID, const Vector3f& pos, const Vector3f& rot) {
	std::cout 	<< "Capacity: " << std::get<vector<Transformable>>(map[TRANSFORMABLE_TYPE]).capacity() 	<< std::endl
				<< "Size: " 	<< std::get<vector<Transformable>>(map[TRANSFORMABLE_TYPE]).size() 		<< std::endl;

	return createComponent(std::get<vector<Transformable>>(map[TRANSFORMABLE_TYPE]), e_type, e_ID, pos, rot);
}

Velocity& Storage::createVelocity(const EntityType e_type, const std::size_t e_ID, const float speed, const float acceleration) {
	std::cout 	<< "Capacity: " << std::get<vector<Velocity>>(map[VELOCITY_TYPE]).capacity() 	<< std::endl
				<< "Size: " 	<< std::get<vector<Velocity>>(map[VELOCITY_TYPE]).size() 		<< std::endl;

	return createComponent(std::get<vector<Velocity>>(map[VELOCITY_TYPE]), e_type, e_ID, speed, acceleration);
}

Velocity& Storage::createVelocity(const EntityType e_type, const std::size_t e_ID, const Vector3f& dir, const float speed, const float acceleration) {
	std::cout 	<< "Capacity: " << std::get<vector<Velocity>>(map[VELOCITY_TYPE]).capacity() 	<< std::endl
				 << "Size: " 	<< std::get<vector<Velocity>>(map[VELOCITY_TYPE]).size() 		<< std::endl;

	return createComponent(std::get<vector<Velocity>>(map[VELOCITY_TYPE]), e_type, e_ID, dir, speed, acceleration);
}

BoundingBox& Storage::createBoundingBox(const EntityType e_type, const std::size_t e_ID, const Vector3f& dim, Vector3f& pos, Vector3f& vel, const bool passable, const ColliderType type, const bool canMutate) {
	if (type == ColliderType::RAY) {
		std::cout 	<< "Capacity: " << std::get<vector<BoundingBox>>(map[RAY_BOUNDING_BOX_TYPE]).capacity() 	<< std::endl
					<< "Size: " 	<< std::get<vector<BoundingBox>>(map[RAY_BOUNDING_BOX_TYPE]).size() 		<< std::endl;

		return createComponent(std::get<vector<BoundingBox>>(map[RAY_BOUNDING_BOX_TYPE]), e_type, e_ID, dim, pos, vel, passable, type);
	}

	if (canMutate || type == DYNAMIC) {
		std::cout 	<< "Capacity: " << std::get<vector<BoundingBox>>(map[SPECIAL_BOUNDING_BOX_TYPE]).capacity() 	<< std::endl
					 << "Size: " 	<< std::get<vector<BoundingBox>>(map[SPECIAL_BOUNDING_BOX_TYPE]).size() 		<< std::endl;

		return createComponent(std::get<vector<BoundingBox>>(map[SPECIAL_BOUNDING_BOX_TYPE]), e_type, e_ID, dim, pos, vel, passable, type);
	}
	else {
		std::cout 	<< "Capacity: " << std::get<vector<BoundingBox>>(map[STATIC_BOUNDING_BOX_TYPE]).capacity() 	<< std::endl
					<< "Size: " 	<< std::get<vector<BoundingBox>>(map[STATIC_BOUNDING_BOX_TYPE]).size() 		<< std::endl;

		return createComponent(std::get<vector<BoundingBox>>(map[STATIC_BOUNDING_BOX_TYPE]), e_type, e_ID, dim, pos, vel, passable, type);
	}
}

Physics& Storage::createPhysics(const EntityType e_type, const std::size_t e_ID, const Vector3f& pos, const Vector3f& vel, const Vector3f& rot) {
	std::cout 	<< "Capacity: " << std::get<vector<Physics>>(map[PHYSICS_TYPE]).capacity() 	<< std::endl
				<< "Size: " 	<< std::get<vector<Physics>>(map[PHYSICS_TYPE]).size() 		<< std::endl;

	return createComponent(std::get<vector<Physics>>(map[PHYSICS_TYPE]), e_type, e_ID, pos, vel, rot);
}

BulletData& Storage::createBulletData(const EntityType e_type, const std::size_t e_ID, const float speed, const bool type, const float damage) {
	std::cout 	<< "Capacity: " << std::get<vector<BulletData>>(map[BULLET_DATA_TYPE]).capacity() 	<< std::endl
				<< "Size: " 	<< std::get<vector<BulletData>>(map[BULLET_DATA_TYPE]).size() 		<< std::endl;

	return createComponent(std::get<vector<BulletData>>(map[BULLET_DATA_TYPE]), e_type, e_ID, speed, type, damage);
}

CharacterData& Storage::createCharacterData(const EntityType e_type, const std::size_t e_ID, const bool mode, const float health, const float attackDamage, const float attackingCooldown) {
	std::cout 	<< "Capacity: " << std::get<vector<CharacterData>>(map[CHARACTER_DATA_TYPE]).capacity() 	<< std::endl
				<< "Size: " 	<< std::get<vector<CharacterData>>(map[CHARACTER_DATA_TYPE]).size() 		<< std::endl;

	return createComponent(std::get<vector<CharacterData>>(map[CHARACTER_DATA_TYPE]), e_type, e_ID, mode, health, attackDamage, attackingCooldown);
}

template<typename T, typename... Args>
T& Storage::createComponent(std::vector<T>& vector, Args&&... args) {
	for (auto& cmp : vector)
		if (!cmp)
			return *(new (&cmp) T(args...));

	return vector.emplace_back(args...);
}

const variantComponentVectorTypes& Storage::getComponents(const ComponentType type) const {
	return const_cast<std::unordered_map<ComponentType,variantComponentVectorTypes>&>(map)[type];
}

variantComponentVectorTypes& Storage::getComponents(const ComponentType type) {
	return const_cast<variantComponentVectorTypes&>(std::as_const(*this).getComponents(type));
}

template<typename T, typename... Args>
T& Storage::createComponent(ComponentType type, Args &&... args) {
	switch(type) {
		case AI_TYPE:
			return createComponent(std::get<vector<AI>>(map[type]), args...);

		case NODE_TYPE:
			return createComponent(std::get<vector<Node_ptr>>(map[type]), args...);

		case VELOCITY_TYPE:
			return createComponent(std::get<vector<Velocity>>(map[type]), args...);

		case TRANSFORMABLE_TYPE:
			return createComponent(std::get<vector<Transformable>>(map[type]), args...);

		case PHYSICS_TYPE:
			return createComponent(std::get<vector<Physics>>(map[type]), args...);

		case CHARACTER_DATA_TYPE:
			return createComponent(std::get<vector<CharacterData>>(map[type]), args...);

		case BULLET_DATA_TYPE:
			return createComponent(std::get<vector<BulletData>>(map[type]), args...);

		case SPECIAL_BOUNDING_BOX_TYPE:
			return createComponent(std::get<vector<BoundingBox>>(map[type]), args...);

		case STATIC_BOUNDING_BOX_TYPE:
			return createComponent(std::get<vector<BoundingBox>>(map[type]), args...);

		case RAY_BOUNDING_BOX_TYPE:
			return createComponent(std::get<vector<BoundingBox>>(map[type]), args...);
	}
}
