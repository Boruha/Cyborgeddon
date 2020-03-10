#pragma once

#include <Engine/util/shaders/Shader.hpp>

constexpr std::string_view PLAIN_VERTEX_PATH {
		"./plain/vertex.glsl"
};

constexpr std::string_view PLAIN_FRAGMENT_PATH {
		"./plain/fragment.glsl"
};

constexpr std::string_view PLAIN_GEMOTRY_PATH {

};

constexpr std::string_view TEXTURE_VERTEX_PATH {
		"./texture/vertex.glsl"
};

constexpr std::string_view TEXTURE_FRAGMENT_PATH {
		"./texture/fragment.glsl"
};

constexpr std::string_view TEXTURE_GEMOTRY_PATH {

};

constexpr std::string_view TRANSFORM_VERTEX_PATH {
		"./transform/vertex.glsl"
};

constexpr std::string_view TRANSFORM_FRAGMENT_PATH {
		"./transform/fragment.glsl"
};

constexpr std::string_view TRANSFORM_GEMOTRY_PATH {

};

constexpr std::string_view MVP_VERTEX_PATH {
		"./mvp/vertex.glsl"
};

constexpr std::string_view MVP_FRAGMENT_PATH {
		"./mvp/fragment.glsl"
};

constexpr std::string_view MVP_GEMOTRY_PATH {

};

constexpr std::string_view MODEL_VERTEX_PATH {
		"./src/Engine/util/shaders/model/vertex.glsl"
};

constexpr std::string_view MODEL_FRAGMENT_PATH {
		"./src/Engine/util/shaders/model/fragment.glsl"
};

constexpr std::string_view MODEL_GEMOTRY_PATH {

};

constexpr std::string_view PLAIN_SHADER_PATH [SHADER_TYPES] {
		PLAIN_VERTEX_PATH,
		PLAIN_FRAGMENT_PATH,
		PLAIN_GEMOTRY_PATH
};

constexpr std::string_view TEXTURE_SHADER_PATH [SHADER_TYPES] {
		TEXTURE_VERTEX_PATH,
		TEXTURE_FRAGMENT_PATH,
		TEXTURE_GEMOTRY_PATH
};

constexpr std::string_view TRANSFORM_SHADER_PATH [SHADER_TYPES] {
		TRANSFORM_VERTEX_PATH,
		TRANSFORM_FRAGMENT_PATH,
		TRANSFORM_GEMOTRY_PATH
};

constexpr std::string_view MVP_SHADER_PATH [SHADER_TYPES] {
		MVP_VERTEX_PATH,
		MVP_FRAGMENT_PATH,
		MVP_GEMOTRY_PATH
};

constexpr std::string_view MODEL_SHADER_PATH [SHADER_TYPES] {
		MODEL_VERTEX_PATH,
		MODEL_FRAGMENT_PATH,
		MODEL_GEMOTRY_PATH
};