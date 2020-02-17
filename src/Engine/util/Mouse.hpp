#pragma once

#include <glm/glm.hpp>

struct Mouse {
    glm::vec2 position;
    bool leftPressed  { false };
    bool rightPressed { false };
};