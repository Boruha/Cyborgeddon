#pragma once

#include <glm/glm.hpp>

using glm::vec3;
using glm::mat4x4;

struct IEntity {
    virtual ~IEntity() = default;

    virtual void render(const mat4x4&) const = 0;
};