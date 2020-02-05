#pragma once

#include <Engine/EngineInterface/SceneInterface/Scene.hpp>

namespace irr::scene {
    struct ISceneManager;
}

struct IrrlichtScene : Scene {

    explicit IrrlichtScene(irr::scene::ISceneManager * sceneManager) : sceneManager(sceneManager) { }
    ~IrrlichtScene() override = default;

    std::unique_ptr<ObjectNode> addObjectNode(  const glm::vec3 * pos,
                                                const glm::vec3 * rot,
                                                const glm::vec3 * sca)
                                                const final;

    std::unique_ptr<CameraNode> addCameraNode(  const glm::vec3 * pos,
                                                const glm::vec3 * rot,
                                                const glm::vec3 * sca,
                                                const glm::vec3 * tar)
                                                const final;
    
    private :
        irr::scene::ISceneManager * sceneManager { nullptr };
};