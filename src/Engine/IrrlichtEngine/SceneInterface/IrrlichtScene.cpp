#include <Engine/IrrlichtEngine/SceneInterface/IrrlichtScene.hpp>
#include <Engine/IrrlichtEngine/SceneInterface/IrrlichtObjectNode.hpp>
#include <Engine/IrrlichtEngine/SceneInterface/IrrlichtCameraNode.hpp>

#include <irrlicht/ISceneManager.h>
#include <irrlicht/IVideoDriver.h>

std::unique_ptr<IObjectNode> IrrlichtScene::addObjectNode(const vec3 * pos, const vec3 * rot, const vec3 * sca) const {
    return std::make_unique<IrrlichtObjectNode>(sceneManager, pos, rot, sca);
}

std::unique_ptr<ICameraNode> IrrlichtScene::addCameraNode(const vec3 * pos, const vec3 * rot, const vec3 * sca, const vec3 * tar) const {
    return std::make_unique<IrrlichtCameraNode>(sceneManager, pos, rot, sca, tar);
}

void IrrlichtScene::loadTexture(const char * const path) const {
	sceneManager->getVideoDriver()->getTexture(path);
}

void IrrlichtScene::unloadTextures() const {
	sceneManager->getVideoDriver()->removeAllTextures();
}
