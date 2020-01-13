#include <cmp/Storage.hpp>
#include <SunlightEngine/SceneNode.hpp>
#include <SunlightEngine/CameraNode.hpp>

using Sun::SceneNode;
using Sun::CameraNode;

Storage::Storage(std::size_t initialSize) {
	AIComponents.reserve(initialSize);
	nodeComponents.reserve(initialSize);
	transformableComponents.reserve(initialSize);
	velocityComponents.reserve(initialSize);
	staticBoundingComponents.reserve(initialSize);
	boundingComponents.reserve(initialSize);
	rayBoundingComponents.resize(initialSize);
	physicsComponents.reserve(initialSize);
	bulletDataComponents.reserve(initialSize);
	characterDataComponents.reserve(initialSize);
}

Storage::~Storage() {
	characterDataComponents.clear();
	bulletDataComponents.clear();
	physicsComponents.clear();
	rayBoundingComponents.clear();
	boundingComponents.clear();
	staticBoundingComponents.clear();
	velocityComponents.clear();
	transformableComponents.clear();
	nodeComponents.clear();
	AIComponents.clear();
}

AI& Storage::createAI(const EntityType e_type, const std::size_t e_ID, const vector<Vector3f>& patrol) {
	return AIComponents.emplace_back(e_type, e_ID, patrol);
}

Node_ptr& Storage::createSceneNode(const Device& device, const Vector3f& position, const Vector3f& rotation, const Vector3f& dim, const char* mesh, const char* texture) {
	return nodeComponents.emplace_back(std::make_unique<SceneNode>(device, position, rotation, dim, mesh, texture));
}

Node_ptr& Storage::createCameraNode(const Device& device, const Vector3f& position, const Vector3f &target) {
	return nodeComponents.emplace_back(std::make_unique<CameraNode>(device, target, position));
}

Transformable& Storage::createTransformable(const EntityType e_type, const std::size_t e_ID, const Vector3f& pos, const Vector3f& rot) {
	return transformableComponents.emplace_back(Transformable(e_type, e_ID, pos, rot));
}

Velocity& Storage::createVelocity(const EntityType e_type, const std::size_t e_ID, const float speed, const float acceleration) {
	return velocityComponents.emplace_back(Velocity(e_type, e_ID, speed, acceleration));
}

Velocity& Storage::createVelocity(EntityType e_type, std::size_t e_ID, const Vector3f& dir, float speed, float acceleration) {
	return velocityComponents.emplace_back(Velocity(e_type, e_ID, dir, speed, acceleration));
}

BoundingBox& Storage::createBoundingBox(const EntityType e_type, const std::size_t e_ID, const Vector3f& dim, const Vector3f& pos, Vector3f& vel, const bool passable, const ColliderType type, const bool canMutate) {
	if (type == ColliderType::RAY)
		return rayBoundingComponents.emplace_back(BoundingBox(e_type, e_ID, dim, pos, vel, passable, type));

	if (canMutate || type == DYNAMIC)
		return boundingComponents.emplace_back(BoundingBox(e_type, e_ID, dim, pos, vel, passable, type));
	else
		return staticBoundingComponents.emplace_back(BoundingBox(e_type, e_ID, dim, pos, vel, passable, type));
}

Physics& Storage::createPhysics(const EntityType e_type, const std::size_t e_ID, const Vector3f& pos, const Vector3f& vel, const Vector3f& rot) {
	return physicsComponents.emplace_back(Physics(e_type, e_ID, pos, vel, rot));
}

BulletData& Storage::createBulletData(const EntityType e_type, const std::size_t e_ID, const float speed, const bool type) {
	return bulletDataComponents.emplace_back(BulletData(e_type, e_ID, speed, type));
}

CharacterData& Storage::createCharacterData(const EntityType e_type, const std::size_t e_ID, const bool mode, const int health, const float attackingCooldown) {
	return characterDataComponents.emplace_back(CharacterData(e_type, e_ID, mode, health, attackingCooldown));
}