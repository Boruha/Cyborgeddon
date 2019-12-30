#include <cmp/Storage.hpp>
#include <SunlightEngine/SceneNode.hpp>
#include <SunlightEngine/CameraNode.hpp>

using Sun::SceneNode;
using Sun::CameraNode;

Storage::Storage(std::size_t initialSize) {
	lockComponents.reserve(initialSize);
	AIComponents.reserve(initialSize);
	sceneNodeComponents.reserve(initialSize);
	cameraNodeComponents.reserve(initialSize);
	transformableComponents.reserve(initialSize);
	velocityComponents.reserve(initialSize);
	boundingComponents.reserve(initialSize);
	physicsComponents.reserve(initialSize);
}

Storage::~Storage() {
	physicsComponents.clear();
	boundingComponents.clear();
	velocityComponents.clear();
	transformableComponents.clear();
	cameraNodeComponents.clear();
	AIComponents.clear();
	sceneNodeComponents.clear();
	lockComponents.clear();
}

Lock& Storage::createLock(const EntityType& e_type, const std::size_t& e_ID) {
	return lockComponents.emplace_back(e_type, e_ID); // lock no depende de una entidad
}

AI& Storage::createAI(const EntityType& e_type, const std::size_t& e_ID, const vector<Vector3f>& patrol) {
	return AIComponents.emplace_back(e_type, e_ID, patrol);
}

SceneNode& Storage::createSceneNode(const Device& device, const Vector3f& position, const Vector3f& rotation, const Vector3f& dim, const char* mesh, const char* texture) {
	return sceneNodeComponents.emplace_back(SceneNode(device, position, rotation, dim, mesh, texture));
}

CameraNode& Storage::createCameraNode(const Device& device, const Vector3f& position, const Vector3f &target) {
	return cameraNodeComponents.emplace_back(CameraNode(device, target, position));
}

Transformable& Storage::createTransformable(const EntityType& e_type, const std::size_t& e_ID, const Vector3f& pos, const Vector3f& rot) {
	return transformableComponents.emplace_back(Transformable(e_type, e_ID, pos, rot));
}

Velocity& Storage::createVelocity(const EntityType& e_type, const std::size_t& e_ID, const float& speed) {
	return velocityComponents.emplace_back(Velocity(e_type, e_ID, speed));
}

BoundingBox& Storage::createBoundingBox(const EntityType& e_type, const std::size_t& e_ID, const Vector3f& dim, const Vector3f& pos, const bool is_static) {
	return boundingComponents.emplace_back(BoundingBox(e_type, e_ID, dim, pos, is_static));
}

Physics& Storage::createPhysics(const EntityType& e_type, const std::size_t& e_ID, const Vector3f& pos, const Vector3f& vel, const Vector3f& rot) {
	return physicsComponents.emplace_back(Physics(e_type, e_ID, pos, vel, rot));
}