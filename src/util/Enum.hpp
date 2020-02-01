#pragma once

#include <irrlicht/Keycodes.h>

enum KEY_CODE : unsigned
{
    KEY_SPACE 		= irr::KEY_SPACE,
    KEY_A 			= irr::KEY_KEY_A,
    KEY_D 			= irr::KEY_KEY_D,
    KEY_LSHIFT   	= irr::KEY_LSHIFT,
    KEY_M			= irr::KEY_KEY_M,
    KEY_S 			= irr::KEY_KEY_S,
    KEY_W 			= irr::KEY_KEY_W,
    KEY_LEFT		= irr::KEY_LEFT,
    KEY_RIGHT		= irr::KEY_RIGHT,
    KEY_CODE_COUNT 	= irr::KEY_KEY_CODES_COUNT
};


enum COLOR : unsigned
{
    TRANSPARENT = 0x00000000,
    BLACK       = 0x000000FF,
    RED         = 0xFF0000FF,
    GREEN       = 0x00FF00FF,
    BLUE        = 0x0000FFFF,
    YELLOW      = 0xFFFF00FF,
    MAGENTA     = 0xFF00FFFF,
    CYAN        = 0x00FFFFFF,
    WHITE       = 0xFFFFFFFF
};