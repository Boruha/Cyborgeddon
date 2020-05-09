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
	INode * addFreeLightNode(const vec3& diff, const vec3& spe, const vec3& dir) override { return nullptr; }



	[[nodiscard]] vec3 cursorToWorld(float x, float y, float far) override;
    
    private :
        irr::scene::ISceneManager * sceneManager { nullptr };
};