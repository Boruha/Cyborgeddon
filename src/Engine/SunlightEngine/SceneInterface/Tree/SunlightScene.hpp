#pragma once

#include <Engine/EngineInterface/SceneInterface/IScene.hpp>
#include <Engine/SunlightEngine/SceneInterface/Tree/SceneManager.hpp>

struct SunlightScene : IScene {

	explicit SunlightScene(SceneManager* _sceneManager) : sceneManager(_sceneManager) { };
	~SunlightScene() override = default;

	std::unique_ptr<INode> addObjectNode(std::string_view) override;
	INode * addMeshNode(std::string_view) override;

	std::unique_ptr<INode> addCameraNode() override;
	INode * addFreeCameraNode() override;

	std::unique_ptr<INode> addLightNode() override;
	INode * addFreeLightNode(const glm::vec3& diff, const glm::vec3& spe) override;

	[[nodiscard]] vec3 cursorToWorld(float x, float y, float far) override;

	void loadTexture(std::string_view path) const override {}
	void unloadTexture(std::string_view path) const override {}
	void unloadTextures() const override {}

private :
	SceneManager * sceneManager;
};