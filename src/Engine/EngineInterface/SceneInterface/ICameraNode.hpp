#pragma once

#include <Engine/EngineInterface/SceneInterface/INode.hpp>

using glm::mat4x4;

struct ICameraNode : INode {
    ~ICameraNode() override = default;

    // getters
    [[nodiscard]] virtual const mat4x4 & getProjectionMatrix() = 0;
    [[nodiscard]] virtual const mat4x4 & getViewMatrix()       = 0;
};