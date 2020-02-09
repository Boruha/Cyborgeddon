#include <Engine/IrrlichtEngine/SceneInterface/IrrlichtObjectNode.hpp>

#include <irrlicht/IMeshSceneNode.h>
#include <irrlicht/ISceneManager.h>

IrrlichtObjectNode::IrrlichtObjectNode (
        irr::scene::ISceneManager * const sceneManager,
        const vec3 * const pos,
        const vec3 * const rot,
        const vec3 * const sca
) : meshNode(sceneManager->addCubeSceneNode(1.f)) {
	// meshNode contiene el sceneNode, pero la clase padre que implementa las
	// funcionalidades generales, no lo sabe, asi que creamos al padre
    p_impl = std::make_unique<IrrlichtNodeImpl>(meshNode, pos, rot, sca);
    // y hacemos update para que al crearse aparezca con su transformacion
    // desde el primer momento
    p_impl->update();
    // de momento no quiero que le afecte la luz
    p_impl->affectedByLight(false);
}

IrrlichtObjectNode::operator bool() const {
	return meshNode != nullptr;
}

void IrrlichtObjectNode::remove() {
	p_impl->remove();
	meshNode = nullptr;
}

void IrrlichtObjectNode::update() {
	p_impl->update();
}

void IrrlichtObjectNode::update(const float delta) {
	p_impl->update(delta);
}

const vec3 & IrrlichtObjectNode::getPosition() const {
	return p_impl->getPosition();
}

const vec3 & IrrlichtObjectNode::getRotation() const {
	return p_impl->getRotation();
}

const vec3 & IrrlichtObjectNode::getScale() const {
	return p_impl->getScale();
}

void IrrlichtObjectNode::setPosition(const vec3 & pos) const {
	p_impl->setPosition(pos);
}

void IrrlichtObjectNode::setRotation(const vec3 & rot) const {
	p_impl->setRotation(rot);
}

void IrrlichtObjectNode::setScale(const vec3 & sca) const {
	p_impl->setScale(sca);
}

void IrrlichtObjectNode::setPosition(const float delta) const {
	p_impl->setPosition(delta);
}

void IrrlichtObjectNode::setRotation(const float delta) const {
	p_impl->setRotation(delta);
}

void IrrlichtObjectNode::setScale(const float delta) const {
	p_impl->setScale(delta);
}

void IrrlichtObjectNode::setTexture(const char * const path) const {
	p_impl->setTexture(path);
}

void IrrlichtObjectNode::affectedByLight(const bool affected) const {
	p_impl->affectedByLight(affected);
}

void IrrlichtObjectNode::setMesh(const char * path) {
	// meshNode necesita que le pasen un IMesh * por parametro
	// Este IMesh * se obtiene a partir de un IAnimatedMeshSceneNode *
	// Por que animado? Porque para irrlicht un IMesh * es un frame
	// de un nodo animado. Si ese nodo tiene 1 solo frame, nosotros consideramos
	// que es "estatico". Por eso hay que obtener AnimatedMesh, y de ahi sacar
	// getMesh(0) que seria el primer frame
	meshNode->setMesh(meshNode->getSceneManager()->getMesh(path)->getMesh(0));
}