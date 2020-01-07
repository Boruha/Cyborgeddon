#include <cmp/Storage.hpp>
#include <SunlightEngine/SceneNode.hpp>
#include <SunlightEngine/CameraNode.hpp>

using Sun::SceneNode;
using Sun::CameraNode;

Storage::Storage(std::size_t initialSize) {
	lockComponents.reserve(initialSize);
	AIComponents.reserve(initialSize);
	nodeComponents.reserve(initialSize);
	transformableComponents.reserve(initialSize);
	velocityComponents.reserve(initialSize);
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
	velocityComponents.clear();
	transformableComponents.clear();
	nodeComponents.clear();
	AIComponents.clear();
	lockComponents.clear();
}

Lock& Storage::createLock(const EntityType e_type, const std::size_t e_ID) {
	return lockComponents.emplace_back(e_type, e_ID); // lock no depende de una entidad
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

BoundingBox& Storage::createBoundingBox(const EntityType e_type, const std::size_t e_ID, const Vector3f& dim, const Vector3f& pos, Vector3f& vel, const bool passable, const ColliderType type) {
	if (type == ColliderType::RAY)
		return rayBoundingComponents.emplace_back(BoundingBox(e_type, e_ID, dim, pos, vel, passable, type));
	else
		return boundingComponents.emplace_back(BoundingBox(e_type, e_ID, dim, pos, vel, passable, type));
}

Physics& Storage::createPhysics(const EntityType e_type, const std::size_t e_ID, const Vector3f& pos, const Vector3f& vel, const Vector3f& rot) {
	return physicsComponents.emplace_back(Physics(e_type, e_ID, pos, vel, rot));
}

BulletData& Storage::createBulletData(const EntityType e_type, const std::size_t e_ID, const int lifetime, const bool type) {
	return bulletDataComponents.emplace_back(BulletData(e_type, e_ID, lifetime, type));
}

CharacterData& Storage::createCharacterData(const EntityType e_type, const std::size_t e_ID, const bool mode, const int health, const int attackingCooldown) {
	return characterDataComponents.emplace_back(CharacterData(e_type, e_ID, mode, health, attackingCooldown));
}
