#pragma once

#include <Engine/SunlightEngine/SceneInterface/Tree/TreeNode.hpp>
#include <Engine/SunlightEngine/SceneInterface/Tree/Camera.hpp>
#include <Engine/SunlightEngine/SceneInterface/Tree/Light.hpp>
#include <Engine/util/shaders/Shader.hpp>
#include <Engine/util/shaders/ShaderPath.hpp>

struct SunlightEngine;
struct ResourceManager;

struct SceneManager {
	explicit SceneManager(SunlightEngine * _engine, ResourceManager * _resourceManager) : engine(_engine), resourceManager(_resourceManager) { }

	void render();

	[[nodiscard]] glm::mat4 getViewProjection() const;
	[[nodiscard]] glm::mat4 getView()           const;

	TreeNode * addMeshNode(std::string_view);
	TreeNode * addCameraNode();
	TreeNode * addLightNode(const glm::vec3& amb, const glm::vec3& diff, const glm::vec3& spe);

	[[nodiscard]] TreeNode * getCameraNode() const { return cameraNode; }
	[[nodiscard]] Camera   * getCamera()     const { return camera;     }

	[[nodiscard]] std::array<TreeNode*, max_light_size> getLightNode() const { return lightNodes;  }
	[[nodiscard]] std::array<Light*,    max_light_size> getLight()     const { return lights;      }

	SunlightEngine * getEngine() { return engine; }

private :

	void setLights();

	std::unique_ptr<TreeNode> root { std::make_unique<TreeNode>(*this) };

	Shader shader { PHONG_BASIC_SHADER }; //TODO: add a shader selector;

	TreeNode * cameraNode { nullptr };
	Camera   * camera     { nullptr };

	//deprecate
	//TreeNode * lightNode  { nullptr };
	//Light    * light      { nullptr };

	std::array<TreeNode*, max_light_size> lightNodes;
	std::array<Light*, max_light_size>    lights;
	std::size_t lights_index { 0 };

	mat4 viewProjection {1};
	mat4 view {1};

	SunlightEngine  * engine { nullptr };
	ResourceManager * resourceManager;
};