#pragma once

#include <glm/glm.hpp>

#include <memory>

struct IObjectNode;
struct ICameraNode;
struct INode;
struct IVideo;

using glm::vec3;

struct IScene {
    virtual ~IScene() = default;

    virtual std::unique_ptr<INode> addObjectNode(std::string_view) = 0;
	virtual INode* addMeshNode(std::string_view) = 0;
	
	virtual std::unique_ptr<INode> addCameraNode() = 0;
	virtual INode * addFreeCameraNode() = 0;
	
	virtual std::unique_ptr<INode> addLightNode() = 0;
	virtual INode * addFreeLightNode(const vec3& amb, const vec3& diff, const vec3& spe) = 0;

	virtual void clearScene() = 0;

	[[nodiscard]] virtual vec3 cursorToWorld(float, float, float) = 0;

	ICameraNode * camera { nullptr };
};