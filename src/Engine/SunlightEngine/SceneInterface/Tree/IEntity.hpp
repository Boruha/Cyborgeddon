#pragma once

#include <glm/glm.hpp>

struct ResourceManager;
class  Shader;

struct IEntity {
    virtual ~IEntity() = default;

    virtual void render(const glm::mat4&, Shader) = 0;
};