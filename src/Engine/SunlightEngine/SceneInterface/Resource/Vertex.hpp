#pragma once

#include <glm/glm.hpp>

constexpr unsigned POSITION_SIZE        = 3;
constexpr unsigned NORMAL_SIZE          = 3;
constexpr unsigned TEXCOORD_SIZE        = 2;

constexpr unsigned POSITION_LOCATION    = 0;
constexpr unsigned NORMAL_LOCATION      = 1;
constexpr unsigned TEXCOORD_LOCATION    = 2;

constexpr unsigned POSITION_OFFSET      = 0 * sizeof(float);
constexpr unsigned NORMAL_OFFSET        = POSITION_OFFSET + POSITION_SIZE * sizeof(float);
constexpr unsigned TEXCOORD_OFFSET      = NORMAL_OFFSET + NORMAL_SIZE * sizeof(float);

struct Vertex {
	explicit Vertex(const glm::vec3 pos = glm::vec3(), const glm::vec3 norm = glm::vec3(), const glm::vec2 coord = glm::vec2())
		: position(pos), normal(norm), textureCoord(coord) { }

	const glm::vec3 position {0};
	const glm::vec3 normal {0};
	const glm::vec2 textureCoord {0};
};