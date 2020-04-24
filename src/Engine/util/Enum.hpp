#pragma once

#include <irrlicht/Keycodes.h>

extern "C" {
	//#include <Engine/util/glad/glad.h>
    #include <GL/glew.h>
	#include <GLFW/glfw3.h>
}

/*
enum KEY_CODE : unsigned
{
    KEY_SPACE 		= irr::KEY_SPACE,
    KEY_A 			= irr::KEY_KEY_A,
    KEY_D 			= irr::KEY_KEY_D,
    KEY_LSHIFT      = irr::KEY_LSHIFT,
    KEY_Q			= irr::KEY_KEY_Q,
    KEY_S 			= irr::KEY_KEY_S,
    KEY_P           = irr::KEY_KEY_P,
	KEY_INTRO       = irr::KEY_RETURN,
	KEY_W 			= irr::KEY_KEY_W,
    KEY_CODE_COUNT 	= irr::KEY_KEY_CODES_COUNT
};
*/
enum KEY_CODE : unsigned
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
	KEY_CODE_COUNT 	= GLFW_KEY_LAST
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