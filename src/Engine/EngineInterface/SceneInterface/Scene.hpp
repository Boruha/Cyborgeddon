#pragma once

#include <glm/glm.hpp>

#include <memory>

struct ObjectNode;
struct CameraNode;

struct Scene {
    virtual ~Scene() = default;

    virtual std::unique_ptr<ObjectNode> addObjectNode(const glm::vec3 * , const glm::vec3 * , const glm::vec3 * ) const = 0;
    virtual std::unique_ptr<CameraNode> addCameraNode(const glm::vec3 * , const glm::vec3 * , const glm::vec3 * , const glm::vec3 * ) const = 0;
};