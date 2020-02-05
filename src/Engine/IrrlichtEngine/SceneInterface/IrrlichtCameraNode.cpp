#include <Engine/IrrlichtEngine/SceneInterface/IrrlichtCameraNode.hpp>

#include <irrlicht/ICameraSceneNode.h>
#include <irrlicht/ISceneManager.h>

IrrlichtCameraNode::IrrlichtCameraNode (
        irr::scene::ISceneManager * const sceneManager,
        const glm::vec3 * const pos,
        const glm::vec3 * const rot,
        const glm::vec3 * const sca,
        const glm::vec3 * const tar
) : IrrlichtNode(pos, rot, sca), target(tar) {
    node = cameraNode = sceneManager->addCameraSceneNode();
    setPosition(*pos);
    setRotation(*rot);
    setScale(*sca);
    setTarget(*tar);
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

void IrrlichtCameraNode::update() {
    preupdate();
    IrrlichtNode::update();
    setTarget(*target);
}

void IrrlichtCameraNode::update(float delta) {
    preupdate();
    IrrlichtNode::update(delta);
    setTarget(delta);
}

const glm::vec3 & IrrlichtCameraNode::getTarget() const {
    return *target;
}

const glm::mat4x4 & IrrlichtCameraNode::getProjectionMatrix() const {
    return projectionMatrix;
}

const glm::mat4x4 &IrrlichtCameraNode::getViewMatrix() const {
    return viewMatrix;
}

void IrrlichtCameraNode::setTarget(const glm::vec3 & targ) const {
    cameraNode->setTarget(irr::core::vector3df(targ.x, targ.y, targ.z));
}

void IrrlichtCameraNode::setTarget(float delta) const {
    setTarget(getTarget() + (*target - getTarget()) * delta);
}
