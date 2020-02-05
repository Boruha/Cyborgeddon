#include <Engine/IrrlichtEngine/SceneInterface/IrrlichtObjectNode.hpp>
#include <irrlicht/IMeshSceneNode.h>
#include <irrlicht/ISceneManager.h>

IrrlichtObjectNode::IrrlichtObjectNode (
        irr::scene::ISceneManager * const sceneManager,
        const glm::vec3 * const pos,
        const glm::vec3 * const rot,
        const glm::vec3 * const sca
) : IrrlichtNode(pos, rot, sca) {
    node = meshNode = sceneManager->addCubeSceneNode(1);
    setPosition(*pos);
    setRotation(*rot);
    setScale(*sca);
}

void IrrlichtObjectNode::setMesh(const char * path) const {
    
}
