#pragma once

#include <string>
#include <utility>

constexpr std::string_view  DIFFUSE  = "texture_diffuse";
constexpr std::string_view  SPECULAR = "texture_specular";
constexpr std::string_view  NORMAL   = "texture_normal";
constexpr std::string_view  HEIGHT   = "texture_height";

struct Texture {
	explicit Texture(const unsigned id = 0, const std::string_view _type = std::string_view(), const std::string_view _path = std::string_view())
		: ID(id), type(_type), path(_path) { }

	unsigned ID {0};
	std::string type;
	std::string path;
};