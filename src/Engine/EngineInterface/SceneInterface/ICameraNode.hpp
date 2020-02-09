#pragma once

#include <Engine/EngineInterface/SceneInterface/INode.hpp>

using glm::mat4x4;

struct ICameraNode : INode {
    ~ICameraNode() override = default;

    // getters
    [[nodiscard]] virtual const vec3 &   getTarget()           const = 0;
    [[nodiscard]] virtual const mat4x4 & getProjectionMatrix() const = 0;
    [[nodiscard]] virtual const mat4x4 & getViewMatrix()       const = 0;

    // setters
    virtual void setTarget(const vec3 &) const = 0;
    virtual void setTarget(float) const = 0;
};