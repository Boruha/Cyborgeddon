#pragma once

#include <iostream>

extern "C" {
//	#include <Engine/util/glad/glad.h>
	#include <GLFW/glfw3.h>
}

inline void error(const std::string_view message) {
	std::cerr << message << std::endl;
	glfwTerminate();
	exit(-1);
}