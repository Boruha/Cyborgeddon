#pragma once

#include <Engine/SunlightEngine/SceneInterface/Tree/TreeNode.hpp>
#include <Engine/SunlightEngine/SceneInterface/Tree/Camera.hpp>
#include <Engine/SunlightEngine/SceneInterface/Tree/Light.hpp>

#include <Engine/util/shaders/ShaderPath.hpp>

struct SunlightEngine;
struct ResourceManager;

struct SceneManager {
	explicit SceneManager(SunlightEngine * _engine, ResourceManager * _resourceManager);
	void render();

	[[nodiscard]] glm::mat4 getLightViewProjection() const;	
	[[nodiscard]] glm::mat4 getViewProjection() const;
	[[nodiscard]] glm::mat4 getView()           const;

	TreeNode * addMeshNode(std::string_view);
	TreeNode * addCameraNode();
	TreeNode * addLightNode(const glm::vec3& diff, const glm::vec3& spe, const glm::vec3& dir);
	TreeNode * addAnimatedNode(std::string_view);

	[[nodiscard]] TreeNode * getCameraNode() const { return cameraNode; }
	[[nodiscard]] Camera   * getCamera()     const { return camera;     }

	[[nodiscard]] std::array<TreeNode*, MAX_LIGHT_SIZE> getLightNode() const { return lightNodes;  }
	[[nodiscard]] std::array<Light*,    MAX_LIGHT_SIZE> getLight()     const { return lights;      }

	SunlightEngine * getEngine() { return engine; }

	void clearScene();

private :
	void renderOffcreen();
	void renderShadow(size_t index);
	void renderScene();
	void sendLightsData2ShaderScene();
	void genShadowTexture();
	void lightMatrixConfig();

	std::unique_ptr<TreeNode> root { std::make_unique<TreeNode>(*this) };
	Shader shaders[NUM_SHADERS] { Shader{ PHONG_BASIC_SHADER }, Shader{ SHADOWS_BASIC_SHADER } };
	bool firstTime { true };

	TreeNode * cameraNode { nullptr };
	Camera   * camera     { nullptr };

	std::array<TreeNode*, MAX_LIGHT_SIZE> lightNodes;
	std::array<Light*   , MAX_LIGHT_SIZE> lights;
	//std::array<unsigned , MAX_LIGHT_SIZE> shadowFBOs;
	//std::array<unsigned , MAX_LIGHT_SIZE> shadowSceneTextures;

	std::size_t lights_index {  0  };
    vec3        ambient      { 0.2f };

	mat4 viewProjection      { 1 };
	mat4 view                { 1 };
	mat4 lightViewProjection { 1 };

	SunlightEngine  * engine { nullptr };
	ResourceManager * resourceManager { nullptr };
};