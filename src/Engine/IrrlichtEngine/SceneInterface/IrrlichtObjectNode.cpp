#include <Engine/IrrlichtEngine/SceneInterface/IrrlichtObjectNode.hpp>

#include <irrlicht/IMeshSceneNode.h>
#include <irrlicht/ISceneManager.h>

IrrlichtObjectNode::IrrlichtObjectNode (
        irr::scene::ISceneManager * const sceneManager
) : meshNode(sceneManager->addCubeSceneNode(1.f)) {
	// meshNode contiene el sceneNode, pero la clase padre que implementa las
	// funcionalidades generales, no lo sabe, asi que creamos al padre
    p_impl = std::make_unique<IrrlichtNodeImpl>(meshNode);
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

const vec3 & IrrlichtObjectNode::getPosition() const {
	return p_impl->getPosition();
}

const vec3 & IrrlichtObjectNode::getRotation() const {
	return p_impl->getRotation();
}

const vec3 & IrrlichtObjectNode::getScale() const {
	return p_impl->getScale();
}

void IrrlichtObjectNode::setPosition(const vec3 & pos) {
	p_impl->setPosition(pos);
}

void IrrlichtObjectNode::setRotation(const vec3 & rot) {
	p_impl->setRotation(rot);
}

void IrrlichtObjectNode::setScale(const vec3 & sca) {
	p_impl->setScale(sca);
}
/*
void IrrlichtObjectNode::setTexture(const std::string_view path) const {
	p_impl->setTexture(path);
}
*/
void IrrlichtObjectNode::affectedByLight(const bool affected) const {
	p_impl->affectedByLight(affected);
}

void IrrlichtObjectNode::setMesh(const std::string_view path) {
	// meshNode necesita que le pasen un IMesh * por parametro
	// Este IMesh * se obtiene a partir de un IAnimatedMeshSceneNode *
	// Por que animado? Porque para irrlicht un IMesh * es un frame
	// de un nodo animado. Si ese nodo tiene 1 solo frame, nosotros consideramos
	// que es "estatico". Por eso hay que obtener AnimatedMesh, y de ahi sacar
	// getMesh(0) que seria el primer frame
	meshNode->setMesh(meshNode->getSceneManager()->getMesh(path.data())->getMesh(0));
}