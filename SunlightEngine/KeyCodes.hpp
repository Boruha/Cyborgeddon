#pragma once

#include <irrlicht/Keycodes.h>
#include <Engine/util/Enum.hpp>

struct Keyboard {
    Keyboard() { std::fill(keys, keys + sizeof(keys), false); }

    bool keys[KEY_CODE_COUNT] {  };
};

// TODO: agregar teclas en funcion de las necesidades