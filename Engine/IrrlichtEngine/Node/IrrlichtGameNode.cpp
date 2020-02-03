#include <Engine/IrrlichtEngine/Node/IrrlichtGameNode.hpp>

#include <irrlicht/ISceneNode.h>
#include <irrlicht/ISceneManager.h>
#include <irrlicht/IVideoDriver.h>

IrrlichtGameNode::operator bool() const {
    return node != nullptr;
}

void IrrlichtGameNode::remove() {
    if (node)
        node->remove();
    node = nullptr;
}

void IrrlichtGameNode::update() const {
    setPosition(*position);
    setRotation(*rotation);
    setScale(*scale);
}

void IrrlichtGameNode::update(const float delta) const {
    setPosition(delta);
    setRotation(delta);
    setScale(delta);
}

const Vector3f & IrrlichtGameNode::getPosition() const {
    return *position;
}

const Vector3f & IrrlichtGameNode::getRotation() const {
    return *rotation;
}

const Vector3f & IrrlichtGameNode::getScale() const {
    return *scale;
}

void IrrlichtGameNode::setPosition(const Vector3f & pos) const {
    node->setPosition(irr::core::vector3df(pos.x, pos.y, pos.z));
}

void IrrlichtGameNode::setRotation(const Vector3f & rot) const {
    node->setRotation(irr::core::vector3df(rot.x, rot.y, rot.z));
}

void IrrlichtGameNode::setScale(const Vector3f & sca) const {
    node->setScale(irr::core::vector3df(sca.x, sca.y, sca.z));
}

void IrrlichtGameNode::setPosition(const float delta) const {
    setPosition(getPosition() + (*position - getPosition()) * delta);
}

void IrrlichtGameNode::setRotation(const float delta) const {
    setRotation(getRotation() + (*rotation - getRotation()) * delta);
}

void IrrlichtGameNode::setScale(const float delta) const {
    setScale(getScale() + (*scale - getScale()) * delta);
}

void IrrlichtGameNode::setTexture(const char * path) const {
    node->setMaterialTexture(0, node->getSceneManager()->getVideoDriver()->getTexture(path));
}

void IrrlichtGameNode::affectedByLight(const bool affected) const {
    node->setMaterialFlag(irr::video::EMF_LIGHTING, affected);
}