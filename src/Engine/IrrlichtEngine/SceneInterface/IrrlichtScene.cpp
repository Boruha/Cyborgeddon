#include <Engine/IrrlichtEngine/SceneInterface/IrrlichtScene.hpp>
#include <Engine/IrrlichtEngine/SceneInterface/IrrlichtObjectNode.hpp>
#include <Engine/IrrlichtEngine/SceneInterface/IrrlichtCameraNode.hpp>

std::unique_ptr<ObjectNode> IrrlichtScene::addObjectNode(const glm::vec3 * pos, const glm::vec3 * rot, const glm::vec3 * sca) const {
    return std::make_unique<IrrlichtObjectNode>(sceneManager, pos, rot, sca);
}

std::unique_ptr<CameraNode> IrrlichtScene::addCameraNode(const glm::vec3 * pos, const glm::vec3 * rot, const glm::vec3 * sca, const glm::vec3 * tar) const {
    return std::make_unique<IrrlichtCameraNode>(sceneManager, pos, rot, sca, tar);
}
