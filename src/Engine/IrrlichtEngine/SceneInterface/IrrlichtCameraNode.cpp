#include <Engine/IrrlichtEngine/SceneInterface/IrrlichtCameraNode.hpp>

#include <irrlicht/ICameraSceneNode.h>
#include <irrlicht/ISceneManager.h>

IrrlichtCameraNode::IrrlichtCameraNode (
        irr::scene::ISceneManager * const sceneManager,
        const glm::vec3 * const pos,
        const glm::vec3 * const rot,
        const glm::vec3 * const sca,
        const glm::vec3 * const tar
) : cameraNode(sceneManager->addCameraSceneNode()), target(tar) {
    p_impl = std::make_unique<IrrlichtNodeImpl>(cameraNode, pos, rot, sca);
    p_impl->update();
	cameraNode->setTarget(irr::core::vector3df(tar->x, tar->y, tar->z));
}

IrrlichtCameraNode::operator bool() const {
	return cameraNode != nullptr;
}

void IrrlichtCameraNode::remove() {
	p_impl->remove();
	cameraNode = nullptr;
}

void IrrlichtCameraNode::update() {
	preupdate();
	p_impl->update();
	setTarget(*target);
}

void IrrlichtCameraNode::update(const float delta) {
	preupdate();
	p_impl->update(delta);
	setTarget(delta);
}

const glm::vec3 & IrrlichtCameraNode::getPosition() const {
	return p_impl->getPosition();
}

const glm::vec3 & IrrlichtCameraNode::getRotation() const {
	return p_impl->getRotation();
}

const glm::vec3 & IrrlichtCameraNode::getScale() const {
	return p_impl->getScale();
}

void IrrlichtCameraNode::setPosition(const glm::vec3 & pos) const {
	p_impl->setPosition(pos);
}

void IrrlichtCameraNode::setRotation(const glm::vec3 & rot) const {
	p_impl->setRotation(rot);
}

void IrrlichtCameraNode::setScale(const glm::vec3 & sca) const {
	p_impl->setScale(sca);
}

void IrrlichtCameraNode::setPosition(const float delta) const {
	p_impl->setPosition(delta);
}

void IrrlichtCameraNode::setRotation(const float delta) const {
	p_impl->setRotation(delta);
}

void IrrlichtCameraNode::setScale(const float delta) const {
	p_impl->setScale(delta);
}

void IrrlichtCameraNode::setTexture(const char * const path) const {
	p_impl->setTexture(path);
}

void IrrlichtCameraNode::affectedByLight(const bool affected) const {
	p_impl->affectedByLight(affected);
}

void IrrlichtCameraNode::preupdate() {
    auto& pm = cameraNode->getProjectionMatrix();

    projectionMatrix = glm::mat4x4 (
            pm [0], pm [1], pm [2], pm [3],
            pm [4], pm [5], pm [6], pm [7],
            pm [8], pm [9], pm[10], pm[11],
            pm[12], pm[13], pm[14], pm[15]
    );

    auto& vm = cameraNode->getViewMatrix();

    viewMatrix = glm::mat4x4 (
            vm [0], vm [1], vm [2], vm [3],
            vm [4], vm [5], vm [6], vm [7],
            vm [8], vm [9], vm[10], vm[11],
            vm[12], vm[13], vm[14], vm[15]
    );
}

const glm::vec3 & IrrlichtCameraNode::getTarget() const {
    return *target;
}

const glm::mat4x4 & IrrlichtCameraNode::getProjectionMatrix() const {
    return projectionMatrix;
}

const glm::mat4x4 & IrrlichtCameraNode::getViewMatrix() const {
    return viewMatrix;
}

void IrrlichtCameraNode::setTarget(const glm::vec3 & targ) const {
    cameraNode->setTarget(irr::core::vector3df(targ.x, targ.y, targ.z));
}

void IrrlichtCameraNode::setTarget(float delta) const {
    setTarget(getTarget() + (*target - getTarget()) * delta);
}
