#include <Engine/IrrlichtEngine/SceneInterface/IrrlichtNode.hpp>

#include <irrlicht/ISceneManager.h>
#include <irrlicht/IVideoDriver.h>
#include <irrlicht/ISceneNode.h>

IrrlichtNode::operator bool() const {
    return node != nullptr;
}

void IrrlichtNode::remove() {
    if (node)
        node->remove();
    node = nullptr;
}

void IrrlichtNode::update() {
    setPosition(*position);
    setRotation(*rotation);
    setScale(*scale);
}

void IrrlichtNode::update(const float delta) {
    setPosition(delta);
    setRotation(delta);
    setScale(delta);
}

const glm::vec3 & IrrlichtNode::getPosition() const {
    return *position;
}

const glm::vec3 & IrrlichtNode::getRotation() const {
    return *rotation;
}

const glm::vec3 & IrrlichtNode::getScale() const {
    return *scale;
}

void IrrlichtNode::setPosition(const glm::vec3 & pos) const {
    node->setPosition(irr::core::vector3df(pos.x, pos.y, pos.z));
}

void IrrlichtNode::setRotation(const glm::vec3 & rot) const {
    node->setRotation(irr::core::vector3df(rot.x, rot.y, rot.z));
}

void IrrlichtNode::setScale(const glm::vec3 & sca) const {
    node->setScale(irr::core::vector3df(sca.x, sca.y, sca.z));
}

void IrrlichtNode::setPosition(const float delta) const {
    setPosition(getPosition() + (*position - getPosition()) * delta);
}

void IrrlichtNode::setRotation(const float delta) const {
    setRotation(getRotation() + (*rotation - getRotation()) * delta);
}

void IrrlichtNode::setScale(const float delta) const {
    setScale(getScale() + (*scale - getScale()) * delta);
}

void IrrlichtNode::setTexture(const char * path) const {
    node->setMaterialTexture(0, node->getSceneManager()->getVideoDriver()->getTexture(path));
}

void IrrlichtNode::affectedByLight(const bool affected) const {
    node->setMaterialFlag(irr::video::EMF_LIGHTING, affected);
}