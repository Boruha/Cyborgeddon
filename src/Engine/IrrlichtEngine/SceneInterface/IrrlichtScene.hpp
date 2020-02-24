#pragma once

#include <Engine/EngineInterface/SceneInterface/IScene.hpp>

namespace irr::scene {
    struct ISceneManager;
}

struct IrrlichtScene : IScene {

    explicit IrrlichtScene(irr::scene::ISceneManager * sceneManager) : sceneManager(sceneManager) { }
    ~IrrlichtScene() override = default;

    std::unique_ptr<INode> addObjectNode(const vec3 * pos,
											   const vec3 * rot,
											   const vec3 * sca)
											   override;

    std::unique_ptr<INode> addCameraNode(const vec3 * pos,
											   const vec3 * rot,
											   const vec3 * sca,
											   const vec3 * tar)
											   override;

    [[nodiscard]] vec3 cursorToWorld(float x, float y, float far) const override;

	void loadTexture(const std::string& path) const override;
	void unloadTexture(const std::string& path) const override;
	void unloadTextures() const override;
    
    private :
        irr::scene::ISceneManager * sceneManager { nullptr };
};