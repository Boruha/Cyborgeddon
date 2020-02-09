#pragma once

#include <glm/glm.hpp>

#include <memory>

struct IObjectNode;
struct ICameraNode;

using glm::vec3;

struct IScene {
    virtual ~IScene() = default;

    virtual std::unique_ptr<IObjectNode> addObjectNode(const vec3 * , const vec3 * , const vec3 * ) const = 0;
    virtual std::unique_ptr<ICameraNode> addCameraNode(const vec3 * , const vec3 * , const vec3 * , const vec3 * ) const = 0;

    virtual void loadTexture(const char *) const = 0;
	virtual void unloadTextures() const = 0;
};