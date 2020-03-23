#pragma once

#include <Engine/EngineInterface/SceneInterface/IScene.hpp>

namespace irr::scene {
    struct ISceneManager;
}

struct IrrlichtScene final : IScene {

    explicit IrrlichtScene(irr::scene::ISceneManager * sceneManager) : sceneManager(sceneManager) { }
    ~IrrlichtScene() override = default;

    std::unique_ptr<INode> addObjectNode(std::string_view) override;
    INode * addMeshNode(std::string_view) override { return nullptr; }

    std::unique_ptr<INode> addCameraNode() override;
	INode * addFreeCameraNode() override { return nullptr; }

    std::unique_ptr<INode> addLightNode() override;
	INode * addFreeLightNode(const vec3& amb, const vec3& diff, const vec3& spe) override { return nullptr; }



	[[nodiscard]] vec3 cursorToWorld(float x, float y, float far) override;

	void loadTexture(std::string_view path) const override;
	void unloadTexture(std::string_view path) const override;
	void unloadTextures() const override;
    
    private :
        irr::scene::ISceneManager * sceneManager { nullptr };
};