#include <Engine/IrrlichtEngine/SceneInterface/IrrlichtNodeImpl.hpp>

#include <irrlicht/ISceneNode.h>
#include <irrlicht/ISceneManager.h>
#include <irrlicht/IVideoDriver.h>

IrrlichtNodeImpl::IrrlichtNodeImpl(
		irr::scene::ISceneNode * node
) : node(node) {
	// implementar constructor si es necesario
}

IrrlichtNodeImpl::operator bool() const {
    return node != nullptr;
}

void IrrlichtNodeImpl::remove() {
	if (node) {
		node->remove();
		node = nullptr;
	}
}

const vec3 & IrrlichtNodeImpl::getPosition() const {
	return pos;
}

const vec3 &IrrlichtNodeImpl::getRotation() const {
	return rot;
}

const vec3 &IrrlichtNodeImpl::getScale() const {
	return sca;
}

void IrrlichtNodeImpl::setPosition(const vec3 & p) {
	pos = p;
	node->setPosition(irr::core::vector3df(pos.x, pos.y, pos.z));
}

void IrrlichtNodeImpl::setRotation(const vec3 & r)  {
	rot = r;
	node->setRotation(irr::core::vector3df(rot.x, rot.y, rot.z));
}

void IrrlichtNodeImpl::setScale(const vec3 & s) {
	sca = s;
	node->setScale(irr::core::vector3df(sca.x, sca.y, sca.z));
}
/*
void IrrlichtNodeImpl::setTexture(const std::string_view path) const {
	node->setMaterialTexture(0, node->getSceneManager()->getVideoDriver()->getTexture(path.data()));
}
*/
void IrrlichtNodeImpl::affectedByLight(const bool enableLight) const {
	node->setMaterialFlag(irr::video::EMF_LIGHTING, enableLight);
}
