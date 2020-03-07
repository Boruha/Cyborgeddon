#include <Engine/IrrlichtEngine/SceneInterface/IrrlichtCameraNode.hpp>

#include <irrlicht/ICameraSceneNode.h>
#include <irrlicht/ISceneManager.h>
#include <iostream>
#include <glm/gtx/string_cast.hpp>

IrrlichtCameraNode::IrrlichtCameraNode (
        irr::scene::ISceneManager * const sceneManager
) : cameraNode(sceneManager->addCameraSceneNode()) {
    p_impl = std::make_unique<IrrlichtNodeImpl>(cameraNode);
}

IrrlichtCameraNode::operator bool() const {
	return cameraNode != nullptr;
}

void IrrlichtCameraNode::remove() {
	p_impl->remove();
	cameraNode = nullptr;
}

const vec3 & IrrlichtCameraNode::getPosition() const {
	return p_impl->getPosition();
}

const vec3 & IrrlichtCameraNode::getRotation() const {
	return p_impl->getRotation();
}

const vec3 & IrrlichtCameraNode::getScale() const {
	return p_impl->getScale();
}

void IrrlichtCameraNode::setPosition(const vec3 & pos) {
	p_impl->setPosition(pos);
}

void IrrlichtCameraNode::setRotation(const vec3 & rot) {
	p_impl->setRotation(rot);
}

void IrrlichtCameraNode::setScale(const vec3 & sca) {
	p_impl->setScale(sca);
}

void IrrlichtCameraNode::setTexture(const std::string_view path) const {
	p_impl->setTexture(path.data());
}

void IrrlichtCameraNode::affectedByLight(const bool affected) const {
	p_impl->affectedByLight(affected);
}

const vec3 & IrrlichtCameraNode::getTarget() const {
    return target;
}

const mat4x4 & IrrlichtCameraNode::getProjectionMatrix() {
	const auto& pm = cameraNode->getProjectionMatrix();

	projectionMatrix = mat4x4 (
			pm [0], pm [1], pm [2], pm [3],
			pm [4], pm [5], pm [6], pm [7],
			pm [8], pm [9], pm[10], pm[11],
			pm[12], pm[13], pm[14], pm[15]
	);

    return projectionMatrix;
}

const mat4x4 & IrrlichtCameraNode::getViewMatrix()  {
	const auto& vm = cameraNode->getViewMatrix();

	viewMatrix = mat4x4 (
			vm [0], vm [1], vm [2], vm [3],
			vm [4], vm [5], vm [6], vm [7],
			vm [8], vm [9], vm[10], vm[11],
			vm[12], vm[13], vm[14], vm[15]
	);

    return viewMatrix;
}

void IrrlichtCameraNode::setTarget(const vec3 & targ) {
	target = targ;
    cameraNode->setTarget(irr::core::vector3df(targ.x, targ.y, targ.z));
}
