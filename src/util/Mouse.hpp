#pragma once

#include <util/Vector2.hpp>

struct Mouse {
    Vector2u position;
    bool leftPressed  { false };
    bool rightPressed { false };
};