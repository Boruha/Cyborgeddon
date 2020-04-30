#pragma once

#include <Engine/util/shaders/Shader.hpp>

constexpr unsigned MAX_LIGHT_SIZE   = 7;
constexpr unsigned NUM_SHADERS      = 3;
constexpr unsigned SHADOW_VP_WIDTH  = 1024;
constexpr unsigned SHADOW_VP_HEIGHT = 1024;

//Phong basic
constexpr std::string_view PHONG_BASIC_VERTEX 
{
		"src/Engine/util/shaders/Phong_Basic/vertex.glsl"
};

constexpr std::string_view PHONG_BASIC_FRAGMENT 
{
		"src/Engine/util/shaders/Phong_Basic/fragment.glsl"
};

constexpr std::string_view PHONG_BASIC_SHADER [SHADER_TYPES] 
{
		PHONG_BASIC_VERTEX,
		"",
		PHONG_BASIC_FRAGMENT
};

//Shadows basic
constexpr std::string_view SHADOWS_BASIC_VERTEX 
{
		"src/Engine/util/shaders/Shadows_Basic/vertex.glsl"
};

constexpr std::string_view SHADOWS_BASIC_FRAGMENT 
{
		"src/Engine/util/shaders/Shadows_Basic/fragment.glsl"
};

constexpr std::string_view SHADOWS_BASIC_GEOMETRY
{
		"src/Engine/util/shaders/Shadows_Basic/geometry.glsl"
};

constexpr std::string_view SHADOWS_BASIC_SHADER [SHADER_TYPES] 
{
		SHADOWS_BASIC_VERTEX,
		"",
		SHADOWS_BASIC_FRAGMENT
};

//test basic
constexpr std::string_view TEST_BASIC_VERTEX 
{
		"src/Engine/util/shaders/test/vertex.glsl"
};

constexpr std::string_view TEST_BASIC_FRAGMENT 
{
		"src/Engine/util/shaders/test/fragment.glsl"
};

constexpr std::string_view TEST_BASIC_SHADER [SHADER_TYPES] 
{
		TEST_BASIC_VERTEX,
		"",
		TEST_BASIC_FRAGMENT
};