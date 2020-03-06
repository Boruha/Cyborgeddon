#pragma once

#include <Engine/SunlightEngine/SceneInterface/Tree/TreeNode.hpp>

#include <Engine/SunlightEngine/SceneInterface/Tree/Camera.hpp>

struct SunlightEngine;
struct ResourceManager;

struct SceneManager {
	explicit SceneManager(SunlightEngine* _engine, ResourceManager* _resourceManager) : engine(_engine), resourceManager(_resourceManager) { }

	void render();

	[[nodiscard]] glm::mat4 getViewProjection() const;

	TreeNode * addMeshNode(std::string_view);
	TreeNode * addCameraNode();

	void setCamera(Camera * cam);

	[[nodiscard]] Camera * getCamera() const { return camera; }

	SunlightEngine * getEngine() { return engine; }

private :
	std::unique_ptr<TreeNode> root { std::make_unique<TreeNode>(*this) };

	TreeNode * cameraNode { nullptr };
	Camera * camera { nullptr };

	SunlightEngine * engine { nullptr };
	ResourceManager* resourceManager;
};