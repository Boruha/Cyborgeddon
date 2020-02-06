#pragma once

#include <Engine/EngineInterface/SceneInterface/INode.hpp>

struct ICameraNode : INode {
    ~ICameraNode() override = default;

    // getters
    [[nodiscard]] virtual const glm::vec3 &   getTarget()           const = 0;
    [[nodiscard]] virtual const glm::mat4x4 & getProjectionMatrix() const = 0;
    [[nodiscard]] virtual const glm::mat4x4 & getViewMatrix()       const = 0;

    // setters
    virtual void setTarget(const glm::vec3 &) const = 0;
    virtual void setTarget(float) const = 0;
};