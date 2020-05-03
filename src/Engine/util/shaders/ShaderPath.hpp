#pragma once

#include <Engine/util/shaders/Shader.hpp>

constexpr unsigned max_light_size = 7;

//Phong basic
constexpr std::string_view PHONG_BASIC_VERTEX {
		"src/Engine/util/shaders/Phong_Basic/vertex.glsl"
};

constexpr std::string_view PHONG_BASIC_FRAGMENT {
		"src/Engine/util/shaders/Phong_Basic/fragment.glsl"
};

constexpr std::string_view PHONG_BASIC_SHADER [SHADER_TYPES] {
		PHONG_BASIC_VERTEX,
		PHONG_BASIC_FRAGMENT
};