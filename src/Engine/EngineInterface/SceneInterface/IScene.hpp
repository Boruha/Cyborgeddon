#pragma once

#include <glm/glm.hpp>

#include <memory>

struct IObjectNode;
struct ICameraNode;

struct IScene {
    virtual ~IScene() = default;

    virtual std::unique_ptr<IObjectNode> addObjectNode(const glm::vec3 * , const glm::vec3 * , const glm::vec3 * ) const = 0;
    virtual std::unique_ptr<ICameraNode> addCameraNode(const glm::vec3 * , const glm::vec3 * , const glm::vec3 * , const glm::vec3 * ) const = 0;
};