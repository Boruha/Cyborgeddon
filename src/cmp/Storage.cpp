#include <cmp/Storage.hpp>
#include <SunlightEngine/SceneNode.hpp>
#include <SunlightEngine/CameraNode.hpp>

using Sun::SceneNode;
using Sun::CameraNode;

Storage::Storage(std::size_t initialSize) {
	std::get<vector<AI>>(map[AI_TYPE] = std::move(vector<AI>())).reserve(initialSize);
	std::get<vector<Node_ptr>>(map[NODE_TYPE] = std::move(vector<Node_ptr>())).reserve(initialSize);
	std::get<vector<Transformable>>(map[TRANSFORMABLE_TYPE] = std::move(vector<Transformable>())).reserve(initialSize);
	std::get<vector<Velocity>>(map[VELOCITY_TYPE] = std::move(vector<Velocity>())).reserve(initialSize);
	std::get<vector<BoundingBox>>(map[STATIC_BOUNDING_BOX_TYPE] = std::move(vector<BoundingBox>())).reserve(initialSize);
	std::get<vector<BoundingBox>>(map[SPECIAL_BOUNDING_BOX_TYPE] = std::move(vector<BoundingBox>())).reserve(initialSize);
	std::get<vector<BoundingBox>>(map[RAY_BOUNDING_BOX_TYPE] = std::move(vector<BoundingBox>())).reserve(initialSize);
	std::get<vector<Physics>>(map[PHYSICS_TYPE] = std::move(vector<Physics>())).reserve(initialSize);
	std::get<vector<BulletData>>(map[BULLET_DATA_TYPE] = std::move(vector<BulletData>())).reserve(initialSize);
	std::get<vector<CharacterData>>(map[CHARACTER_DATA_TYPE] = std::move(vector<CharacterData>())).reserve(initialSize);
}

Storage::~Storage() {
	map.clear();
}

AI& Storage::createAI(const EntityType e_type, const std::size_t e_ID, const vector<Vector3f>& patrol) {
	return std::get<vector<AI>>(map[AI_TYPE]).emplace_back(AI(e_type, e_ID, patrol));
}

Node_ptr& Storage::createSceneNode(const Device& device, const Vector3f& position, const Vector3f& rotation, const Vector3f& dim, const char* mesh, const char* texture) {
	return std::get<vector<Node_ptr>>(map[NODE_TYPE]).emplace_back(std::make_unique<SceneNode>(device, position, rotation, dim, mesh, texture));
}

Node_ptr& Storage::createCameraNode(const Device& device, const Vector3f& position, const Vector3f &target) {
	return std::get<vector<Node_ptr>>(map[NODE_TYPE]).emplace_back(std::make_unique<CameraNode>(device, target, position));
}

Transformable& Storage::createTransformable(const EntityType e_type, const std::size_t e_ID, const Vector3f& pos, const Vector3f& rot) {
	return std::get<vector<Transformable>>(map[TRANSFORMABLE_TYPE]).emplace_back(Transformable(e_type, e_ID, pos, rot));
}

Velocity& Storage::createVelocity(const EntityType e_type, const std::size_t e_ID, const float speed, const float acceleration) {
	return std::get<vector<Velocity>>(map[VELOCITY_TYPE]).emplace_back(Velocity(e_type, e_ID, speed, acceleration));
}

Velocity& Storage::createVelocity(EntityType e_type, std::size_t e_ID, const Vector3f& dir, float speed, float acceleration) {
	return std::get<vector<Velocity>>(map[VELOCITY_TYPE]).emplace_back(Velocity(e_type, e_ID, dir, speed, acceleration));
}

BoundingBox& Storage::createBoundingBox(const EntityType e_type, const std::size_t e_ID, const Vector3f& dim, const Vector3f& pos, Vector3f& vel, const bool passable, const ColliderType type, const bool canMutate) {
	if (type == ColliderType::RAY)
		return std::get<vector<BoundingBox>>(map[RAY_BOUNDING_BOX_TYPE]).emplace_back(BoundingBox(e_type, e_ID, dim, pos, vel, passable, type));

	if (canMutate || type == DYNAMIC)
		return std::get<vector<BoundingBox>>(map[SPECIAL_BOUNDING_BOX_TYPE]).emplace_back(BoundingBox(e_type, e_ID, dim, pos, vel, passable, type));
	else
		return std::get<vector<BoundingBox>>(map[STATIC_BOUNDING_BOX_TYPE]).emplace_back(BoundingBox(e_type, e_ID, dim, pos, vel, passable, type));
}

Physics& Storage::createPhysics(const EntityType e_type, const std::size_t e_ID, const Vector3f& pos, const Vector3f& vel, const Vector3f& rot) {
	return std::get<vector<Physics>>(map[PHYSICS_TYPE]).emplace_back(Physics(e_type, e_ID, pos, vel, rot));
}

BulletData& Storage::createBulletData(const EntityType e_type, const std::size_t e_ID, const float speed, const bool type, const float damage) {
	return std::get<vector<BulletData>>(map[BULLET_DATA_TYPE]).emplace_back(BulletData(e_type, e_ID, speed, type, damage));
}

CharacterData& Storage::createCharacterData(const EntityType e_type, const std::size_t e_ID, const bool mode, const float health, const float attackDamage, const float attackingCooldown) {
	return std::get<vector<CharacterData>>(map[CHARACTER_DATA_TYPE]).emplace_back(CharacterData(e_type, e_ID, mode, health, attackDamage, attackingCooldown));
}