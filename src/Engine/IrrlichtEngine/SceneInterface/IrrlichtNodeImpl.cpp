#include <Engine/IrrlichtEngine/SceneInterface/IrrlichtNodeImpl.hpp>

#include <irrlicht/ISceneNode.h>
#include <irrlicht/ISceneManager.h>
#include <irrlicht/IVideoDriver.h>

IrrlichtNodeImpl::IrrlichtNodeImpl(
		irr::scene::ISceneNode * node,
		const vec3 * const pos,
		const vec3 * const rot,
		const vec3 * const sca
) : node(node), position(pos), rotation(rot), scale(sca) {
	// implementar constructor si es necesario
}

void IrrlichtNodeImpl::remove() {
	if (node) {
		node->remove();
		node = nullptr;
	}
}

void IrrlichtNodeImpl::update() {
	setPosition(*position);
	setRotation(*rotation);
	setScale(*scale);
}

void IrrlichtNodeImpl::update(const float delta) {
	setPosition(delta);
	setRotation(delta);
	setScale(delta);
}

const glm::vec3 & IrrlichtNodeImpl::getPosition() const {
	return *position;
}

const glm::vec3 &IrrlichtNodeImpl::getRotation() const {
	return *rotation;
}

const glm::vec3 &IrrlichtNodeImpl::getScale() const {
	return *scale;
}

void IrrlichtNodeImpl::setPosition(const glm::vec3 & pos) const {
	node->setPosition(irr::core::vector3df(pos.x, pos.y, pos.z));
}

void IrrlichtNodeImpl::setRotation(const glm::vec3 & rot) const {
	node->setRotation(irr::core::vector3df(rot.x, rot.y, rot.z));
}

void IrrlichtNodeImpl::setScale(const glm::vec3 & sca) const {
	node->setScale(irr::core::vector3df(sca.x, sca.y, sca.z));
}

void IrrlichtNodeImpl::setPosition(const float delta) const {
	setPosition(getPosition() + (*position - getPosition()) * delta);
}

void IrrlichtNodeImpl::setRotation(const float delta) const {
	setRotation(getRotation() + (*rotation - getRotation()) * delta);
}

void IrrlichtNodeImpl::setScale(const float delta) const {
	setScale(getScale() + (*scale - getScale()) * delta);
}

void IrrlichtNodeImpl::setTexture(const char * const path) const {
	node->setMaterialTexture(0, node->getSceneManager()->getVideoDriver()->getTexture(path));
}

void IrrlichtNodeImpl::affectedByLight(const bool enableLight) const {
	node->setMaterialFlag(irr::video::EMF_LIGHTING, enableLight);
}
