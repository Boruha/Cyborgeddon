#pragma once

#include <Engine/util/Vector2.hpp>

struct Mouse {
    Vector2u position;
    bool leftPressed  { false };
    bool rightPressed { false };
};