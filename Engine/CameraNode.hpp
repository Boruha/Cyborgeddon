#pragma once

#include <Engine/GameNode.hpp>
#include <glm/glm.hpp>

struct CameraNode : GameNode {
    ~CameraNode() override = 0;

    // getters
    [[nodiscard]] virtual Vector3f &    getTarget()           const = 0;
    [[nodiscard]] virtual glm::mat4x4 & getProjectionMatrix() const = 0;
    [[nodiscard]] virtual glm::mat4x4 & getViewMatrix()       const = 0;

    // setters
    void virtual setTarget(const Vector3f &) const = 0;
};