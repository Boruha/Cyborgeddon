#pragma once

#include <Engine/util/shaders/Shader.hpp>

constexpr unsigned MAX_LIGHT_SIZE   = 7;
constexpr unsigned NUM_SHADERS      = 2;
constexpr unsigned SHADOW_VP_WIDTH  = 4096;
constexpr unsigned SHADOW_VP_HEIGHT = 4096;

//Phong basic
constexpr std::string_view PHONG_BASIC_VERTEX 
{
		"../src/Engine/util/shaders/Phong_Basic/vertex.glsl"
};

constexpr std::string_view PHONG_BASIC_FRAGMENT 
{
		"../src/Engine/util/shaders/Phong_Basic/fragment.glsl"
};

constexpr std::string_view PHONG_BASIC_SHADER [SHADER_TYPES] 
{
		PHONG_BASIC_VERTEX,
		"",
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
		"",
		TEXTURE_FRAGMENT
};

//Shadows basic
constexpr std::string_view SHADOWS_BASIC_VERTEX 
{
		"../src/Engine/util/shaders/Shadows_Basic/vertex.glsl"
};

constexpr std::string_view SHADOWS_BASIC_FRAGMENT 
{
		"../src/Engine/util/shaders/Shadows_Basic/fragment.glsl"
};

constexpr std::string_view SHADOWS_BASIC_GEOMETRY
{
		"../src/Engine/util/shaders/Shadows_Basic/geometry.glsl"
};

constexpr std::string_view SHADOWS_BASIC_SHADER [SHADER_TYPES] 
{
		SHADOWS_BASIC_VERTEX,
		"",
		SHADOWS_BASIC_FRAGMENT
};

