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
}

Storage::~Storage() {
	velocityComponents.clear();
	transformableComponents.clear();
	cameraNodeComponents.clear();
	AIComponents.clear();
	sceneNodeComponents.clear();
	lockComponents.clear();
}

Lock& Storage::createLock() {
	return lockComponents.emplace_back();
}

AI& Storage::createAI(const vector<Vector3f>& patrol) {
	return AIComponents.emplace_back(patrol);
}

SceneNode& Storage::createSceneNode(const Device& device, const Vector3f& position, const Vector3f& rotation, const Vector3f& dim, const char* mesh, const char* texture) {
	return sceneNodeComponents.emplace_back(SceneNode(device, position, rotation, dim, mesh, texture));
}

CameraNode& Storage::createCameraNode(const Device& device, const Vector3f& position, const Vector3f &target) {
	return cameraNodeComponents.emplace_back(CameraNode(device, target, position));
}

Transformable& Storage::createTransformable(const Vector3f& pos) {
	return transformableComponents.emplace_back(Transformable(pos));
}

Velocity& Storage::createVelocity(const Vector3f& dir, const float& speed) {
	return velocityComponents.emplace_back(Velocity(dir, speed));
}

Velocity& Storage::createVelocity(const float& speed) {
	return velocityComponents.emplace_back(Velocity(speed));
}
