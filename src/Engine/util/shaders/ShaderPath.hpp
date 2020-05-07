#pragma once

#include <Engine/util/shaders/Shader.hpp>

constexpr unsigned max_light_size = 7;

//Phong basic
constexpr std::string_view PHONG_BASIC_VERTEX {
		"../src/Engine/util/shaders/Phong_Basic/vertex.glsl"
};

constexpr std::string_view PHONG_BASIC_FRAGMENT {
		"../src/Engine/util/shaders/Phong_Basic/fragment.glsl"
};

constexpr std::string_view PHONG_BASIC_SHADER [SHADER_TYPES] {
		PHONG_BASIC_VERTEX,
		PHONG_BASIC_FRAGMENT
};


constexpr std::string_view TEXTURE_VERTEX {
		"../src/Engine/util/shaders/Texture/vertex.glsl"
};

constexpr std::string_view TEXTURE_FRAGMENT {
		"../src/Engine/util/shaders/Texture/fragment.glsl"
};

constexpr std::string_view TEXTURE_SHADERS [SHADER_TYPES] {
		TEXTURE_VERTEX,
		TEXTURE_FRAGMENT
};