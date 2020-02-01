#pragma once

#include <algorithm>
#include <Engine/util/Enum.hpp>

struct Keyboard {
    Keyboard() { std::fill(keys, keys + sizeof(keys), false); }

    bool keys[KEY_CODE_COUNT] {  };
};