#pragma once

#include <Engine/EngineInterface/SceneInterface/Node.hpp>

struct CameraNode : virtual Node {
    ~CameraNode() override = default;

    void update() override = 0;
    // updates using interpolation based on parameter
    void update(float) override = 0;

    // getters
    [[nodiscard]] virtual const glm::vec3 &   getTarget()           const = 0;
    [[nodiscard]] virtual const glm::mat4x4 & getProjectionMatrix() const = 0;
    [[nodiscard]] virtual const glm::mat4x4 & getViewMatrix()       const = 0;

    // setters
    virtual void setTarget(const glm::vec3 &) const = 0;

    virtual void setTarget(float) const = 0;
};