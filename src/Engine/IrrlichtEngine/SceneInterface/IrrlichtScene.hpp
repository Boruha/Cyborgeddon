#pragma once

#include <Engine/EngineInterface/SceneInterface/IScene.hpp>

namespace irr::scene {
    struct ISceneManager;
}

struct IrrlichtScene : IScene {

    explicit IrrlichtScene(irr::scene::ISceneManager * sceneManager) : sceneManager(sceneManager) { }
    ~IrrlichtScene() override = default;

    std::unique_ptr<INode> addObjectNode() override;

    std::unique_ptr<INode> addCameraNode() override;

    [[nodiscard]] vec3 cursorToWorld(float x, float y, float far) override;

	void loadTexture(std::string_view path) const override;
	void unloadTexture(std::string_view path) const override;
	void unloadTextures() const override;
    
    private :
        irr::scene::ISceneManager * sceneManager { nullptr };
};