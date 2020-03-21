#pragma once

#include <Engine/SunlightEngine/SceneInterface/Tree/TreeNode.hpp>

#include <Engine/SunlightEngine/SceneInterface/Tree/Camera.hpp>
#include <Engine/SunlightEngine/SceneInterface/Tree/Light.hpp>

struct SunlightEngine;
struct ResourceManager;

struct SceneManager {
	explicit SceneManager(SunlightEngine * _engine, ResourceManager * _resourceManager) : engine(_engine), resourceManager(_resourceManager) { }

	void render();

	[[nodiscard]] glm::mat4 getViewProjection() const;

	TreeNode * addMeshNode(std::string_view);
	TreeNode * addCameraNode();
	TreeNode * addLightNode(float amb, float diff, float spe);

	[[nodiscard]] TreeNode * getCameraNode() const { return cameraNode; }
	[[nodiscard]] Camera   * getCamera()     const { return camera;     }

	[[nodiscard]] TreeNode * getLightNode()  const { return lightNode;  }
	[[nodiscard]] Light    * getLight()      const { return light;      }

	SunlightEngine * getEngine() { return engine; }

private :
	std::unique_ptr<TreeNode> root { std::make_unique<TreeNode>(*this) };

	TreeNode * cameraNode { nullptr };
	Camera   * camera     { nullptr };

	TreeNode * lightNode  { nullptr };
	Light    * light      { nullptr };

	mat4 viewProjection {1};

	SunlightEngine  * engine { nullptr };
	ResourceManager * resourceManager;
};