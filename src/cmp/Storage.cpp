#include <cmp/Storage.hpp>
#include <SunlightEngine/SceneNode.hpp>
#include <SunlightEngine/CameraNode.hpp>

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

	for (unsigned i = 0; i < std::get<vector<AI>>(map[AI_TYPE]).size(); ++i)
		if (std::get<vector<AI>>(map[AI_TYPE]).at(i).getEntityType() == UNDEFINED)
			return std::get<vector<AI>>(map[AI_TYPE]).at(i) = std::move(AI(e_type, e_ID, patrol));

	return std::get<vector<AI>>(map[AI_TYPE]).emplace_back(AI(e_type, e_ID, patrol));
}

Node_ptr& Storage::createSceneNode(const Device& device, const Vector3f& position, const Vector3f& rotation, const Vector3f& dim, const char* mesh, const char* texture) {
	std::cout 	<< "Capacity: " << std::get<vector<Node_ptr>>(map[NODE_TYPE]).capacity() 	<< std::endl
				<< "Size: " 	<< std::get<vector<Node_ptr>>(map[NODE_TYPE]).size() 		<< std::endl;

	for (unsigned  i = 0; i < std::get<vector<Node_ptr >>(map[NODE_TYPE]).size(); ++i)
		if (!std::get<vector<Node_ptr>>(map[NODE_TYPE]).at(i)->isAlive())
			return std::get<vector<Node_ptr>>(map[NODE_TYPE]).at(i) = std::make_unique<SceneNode>(device, position, rotation, dim, mesh, texture);

	return std::get<vector<Node_ptr>>(map[NODE_TYPE]).emplace_back(std::make_unique<SceneNode>(device, position, rotation, dim, mesh, texture));
}

Node_ptr& Storage::createCameraNode(const Device& device, const Vector3f& position, const Vector3f &target) {
	std::cout 	<< "Capacity: " << std::get<vector<Node_ptr>>(map[NODE_TYPE]).capacity() 	<< std::endl
				 << "Size: " 	<< std::get<vector<Node_ptr>>(map[NODE_TYPE]).size() 		<< std::endl;

	for (unsigned i = 0; i < std::get<vector<Node_ptr>>(map[NODE_TYPE]).size(); ++i)
		if (!std::get<vector<Node_ptr>>(map[NODE_TYPE]).at(i)->isAlive())
			return std::get<vector<Node_ptr>>(map[NODE_TYPE]).at(i) = std::make_unique<CameraNode>(device, target, position);

	return std::get<vector<Node_ptr>>(map[NODE_TYPE]).emplace_back(std::make_unique<CameraNode>(device, target, position));
}

Transformable& Storage::createTransformable(const EntityType e_type, const std::size_t e_ID, const Vector3f& pos, const Vector3f& rot) {
	std::cout 	<< "Capacity: " << std::get<vector<Transformable>>(map[TRANSFORMABLE_TYPE]).capacity() 	<< std::endl
				<< "Size: " 	<< std::get<vector<Transformable>>(map[TRANSFORMABLE_TYPE]).size() 		<< std::endl;

	for (unsigned i = 0; i < std::get<vector<Transformable>>(map[TRANSFORMABLE_TYPE]).size(); ++i)
		if (std::get<vector<Transformable>>(map[TRANSFORMABLE_TYPE]).at(i).getEntityType() == UNDEFINED)
			return std::get<vector<Transformable>>(map[TRANSFORMABLE_TYPE]).at(i) = std::move(Transformable(e_type, e_ID, pos, rot));

	return std::get<vector<Transformable>>(map[TRANSFORMABLE_TYPE]).emplace_back(Transformable(e_type, e_ID, pos, rot));
}

Velocity& Storage::createVelocity(const EntityType e_type, const std::size_t e_ID, const float speed, const float acceleration) {
	std::cout 	<< "Capacity: " << std::get<vector<Velocity>>(map[VELOCITY_TYPE]).capacity() 	<< std::endl
				<< "Size: " 	<< std::get<vector<Velocity>>(map[VELOCITY_TYPE]).size() 		<< std::endl;

	for (unsigned i = 0; i < std::get<vector<Velocity>>(map[VELOCITY_TYPE]).size(); ++i)
		if (std::get<vector<Velocity>>(map[VELOCITY_TYPE]).at(i).getEntityType() == UNDEFINED)
			return std::get<vector<Velocity>>(map[VELOCITY_TYPE]).at(i) = std::move(Velocity(e_type, e_ID, speed, acceleration));

	return std::get<vector<Velocity>>(map[VELOCITY_TYPE]).emplace_back(Velocity(e_type, e_ID, speed, acceleration));
}

Velocity& Storage::createVelocity(const EntityType e_type, const std::size_t e_ID, const Vector3f& dir, const float speed, const float acceleration) {
	std::cout 	<< "Capacity: " << std::get<vector<Velocity>>(map[VELOCITY_TYPE]).capacity() 	<< std::endl
				 << "Size: " 	<< std::get<vector<Velocity>>(map[VELOCITY_TYPE]).size() 		<< std::endl;

	for (unsigned i = 0; i < std::get<vector<Velocity>>(map[VELOCITY_TYPE]).size(); ++i)
		if (std::get<vector<Velocity>>(map[VELOCITY_TYPE]).at(i).getEntityType() == UNDEFINED)
			return std::get<vector<Velocity>>(map[VELOCITY_TYPE]).at(i) = std::move(Velocity(e_type, e_ID, dir, speed, acceleration));

	return std::get<vector<Velocity>>(map[VELOCITY_TYPE]).emplace_back(Velocity(e_type, e_ID, dir, speed, acceleration));
}

BoundingBox& Storage::createBoundingBox(const EntityType e_type, const std::size_t e_ID, const Vector3f& dim, Vector3f& pos, Vector3f& vel, const bool passable, const ColliderType type, const bool canMutate) {
	if (type == ColliderType::RAY) {
		std::cout 	<< "Capacity: " << std::get<vector<BoundingBox>>(map[RAY_BOUNDING_BOX_TYPE]).capacity() 	<< std::endl
					<< "Size: " 	<< std::get<vector<BoundingBox>>(map[RAY_BOUNDING_BOX_TYPE]).size() 		<< std::endl;

		for (unsigned i = 0; i < std::get<vector<BoundingBox>>(map[RAY_BOUNDING_BOX_TYPE]).size(); ++i)
			if (std::get<vector<BoundingBox>>(map[RAY_BOUNDING_BOX_TYPE]).at(i).getEntityType() == UNDEFINED)
				return std::get<vector<BoundingBox>>(map[RAY_BOUNDING_BOX_TYPE]).at(i) = std::move(BoundingBox(e_type, e_ID, dim, pos, vel, passable, type));

		return std::get<vector<BoundingBox>>(map[RAY_BOUNDING_BOX_TYPE]).emplace_back(BoundingBox(e_type, e_ID, dim, pos, vel, passable, type));
	}

	if (canMutate || type == DYNAMIC) {
		std::cout 	<< "Capacity: " << std::get<vector<BoundingBox>>(map[SPECIAL_BOUNDING_BOX_TYPE]).capacity() 	<< std::endl
					 << "Size: " 	<< std::get<vector<BoundingBox>>(map[SPECIAL_BOUNDING_BOX_TYPE]).size() 		<< std::endl;

		for (unsigned i = 0; i < std::get<vector<BoundingBox>>(map[SPECIAL_BOUNDING_BOX_TYPE]).size(); ++i)
			if (std::get<vector<BoundingBox>>(map[SPECIAL_BOUNDING_BOX_TYPE]).at(i).getEntityType() == UNDEFINED)
				return std::get<vector<BoundingBox>>(map[SPECIAL_BOUNDING_BOX_TYPE]).at(i) = std::move(BoundingBox(e_type, e_ID, dim, pos, vel, passable, type));

		return std::get<vector<BoundingBox>>(map[SPECIAL_BOUNDING_BOX_TYPE]).emplace_back(BoundingBox(e_type, e_ID, dim, pos, vel, passable, type));
	}
	else {
		std::cout 	<< "Capacity: " << std::get<vector<BoundingBox>>(map[STATIC_BOUNDING_BOX_TYPE]).capacity() 	<< std::endl
					<< "Size: " 	<< std::get<vector<BoundingBox>>(map[STATIC_BOUNDING_BOX_TYPE]).size() 		<< std::endl;

		for (unsigned i = 0; i < std::get<vector<BoundingBox>>(map[STATIC_BOUNDING_BOX_TYPE]).size(); ++i)
			if (std::get<vector<BoundingBox>>(map[STATIC_BOUNDING_BOX_TYPE]).at(i).getEntityType() == UNDEFINED)
				return std::get<vector<BoundingBox>>(map[STATIC_BOUNDING_BOX_TYPE]).at(i) = std::move(BoundingBox(e_type, e_ID, dim, pos, vel, passable, type));

		return std::get<vector<BoundingBox>>(map[STATIC_BOUNDING_BOX_TYPE]).emplace_back(BoundingBox(e_type, e_ID, dim, pos, vel, passable, type));
	}
}

Physics& Storage::createPhysics(const EntityType e_type, const std::size_t e_ID, const Vector3f& pos, const Vector3f& vel, const Vector3f& rot) {
	std::cout 	<< "Capacity: " << std::get<vector<Physics>>(map[PHYSICS_TYPE]).capacity() 	<< std::endl
				<< "Size: " 	<< std::get<vector<Physics>>(map[PHYSICS_TYPE]).size() 		<< std::endl;

	for (unsigned i = 0; i < std::get<vector<Physics>>(map[PHYSICS_TYPE]).size(); ++i)
		if (std::get<vector<Physics>>(map[PHYSICS_TYPE]).at(i).getEntityType() == UNDEFINED)
			return std::get<vector<Physics>>(map[PHYSICS_TYPE]).at(i) = std::move(Physics(e_type, e_ID, pos, vel, rot));

	return std::get<vector<Physics>>(map[PHYSICS_TYPE]).emplace_back(Physics(e_type, e_ID, pos, vel, rot));
}

BulletData& Storage::createBulletData(const EntityType e_type, const std::size_t e_ID, const float speed, const bool type, const float damage) {
	std::cout 	<< "Capacity: " << std::get<vector<BulletData>>(map[BULLET_DATA_TYPE]).capacity() 	<< std::endl
				<< "Size: " 	<< std::get<vector<BulletData>>(map[BULLET_DATA_TYPE]).size() 		<< std::endl;

	for (unsigned i = 0; i < std::get<vector<BulletData>>(map[BULLET_DATA_TYPE]).size(); ++i)
		if (std::get<vector<BulletData>>(map[BULLET_DATA_TYPE]).at(i).getEntityType() == UNDEFINED)
			return std::get<vector<BulletData>>(map[BULLET_DATA_TYPE]).at(i) = std::move(BulletData(e_type, e_ID, speed, type, damage));

	return std::get<vector<BulletData>>(map[BULLET_DATA_TYPE]).emplace_back(BulletData(e_type, e_ID, speed, type, damage));
}

CharacterData& Storage::createCharacterData(const EntityType e_type, const std::size_t e_ID, const bool mode, const float health, const float attackDamage, const float attackingCooldown) {
	std::cout 	<< "Capacity: " << std::get<vector<CharacterData>>(map[CHARACTER_DATA_TYPE]).capacity() 	<< std::endl
				<< "Size: " 	<< std::get<vector<CharacterData>>(map[CHARACTER_DATA_TYPE]).size() 		<< std::endl;

	for (unsigned i = 0; i < std::get<vector<CharacterData>>(map[CHARACTER_DATA_TYPE]).size(); ++i)
		if (std::get<vector<CharacterData>>(map[CHARACTER_DATA_TYPE]).at(i).getEntityType() == UNDEFINED)
			return std::get<vector<CharacterData>>(map[CHARACTER_DATA_TYPE]).at(i) = std::move(CharacterData(e_type, e_ID, mode, health, attackDamage, attackingCooldown));

	return std::get<vector<CharacterData>>(map[CHARACTER_DATA_TYPE]).emplace_back(CharacterData(e_type, e_ID, mode, health, attackDamage, attackingCooldown));
}

const variantComponentVectorTypes& Storage::getComponents(const ComponentType type) const {
	return const_cast<std::unordered_map<ComponentType,variantComponentVectorTypes>&>(map)[type];
}

variantComponentVectorTypes& Storage::getComponents(const ComponentType type) {
	return const_cast<variantComponentVectorTypes&>(std::as_const(*this).getComponents(type));
}