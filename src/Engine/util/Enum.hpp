#pragma once

extern "C" {
	#include <Engine/util/glad/glad.h>
	#include <GLFW/glfw3.h>
}

enum
{
	KEY_SPACE 		= GLFW_KEY_SPACE,
	KEY_A 			= GLFW_KEY_A,
	KEY_D 			= GLFW_KEY_D,
	KEY_LSHIFT      = GLFW_KEY_LEFT_SHIFT,
	KEY_Q			= GLFW_KEY_Q,
	KEY_S 			= GLFW_KEY_S,
	KEY_P           = GLFW_KEY_P,
	KEY_INTRO       = GLFW_KEY_ENTER,
	KEY_W 			= GLFW_KEY_W,
	KEY_LEFT        = GLFW_KEY_LEFT,
	KEY_RIGHT       = GLFW_KEY_RIGHT,
	KEY_UP          = GLFW_KEY_UP,
	KEY_DOWN        = GLFW_KEY_DOWN,
	KEY_CODE_COUNT 	= GLFW_KEY_LAST
};

enum
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