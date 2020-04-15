#pragma once

#include <iostream>

inline void error(const std::string_view message) {
	std::cerr << message << std::endl;
	exit(-1);
}