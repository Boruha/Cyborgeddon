#pragma once

#include <glm/glm.hpp>

struct ResourceManager;

struct IEntity {
    virtual ~IEntity() = default;

    virtual void render(const glm::mat4&) = 0;
};