#pragma once

#include <cmp/Component.hpp>
#include <array>

struct TriangleOBB : Component {
	explicit TriangleOBB(const EntityType e_type, const EntityID e_ID, const std::array<vec3, 3>& vtx)
			: Component(e_type, e_ID), vertices(vtx)
	{
		float  minX = std::numeric_limits<float>::infinity();
		float  minZ = std::numeric_limits<float>::infinity();

		float  maxX = - std::numeric_limits<float>::infinity();
		float  maxZ = - std::numeric_limits<float>::infinity();

		for (const auto v : vertices) {
			if (minX > v.x)
				minX = v.x;
			if (minZ > v.z)
				minZ = v.z;

			if (maxX < v.x)
				maxX = v.x;
			if (maxZ < v.z)
				maxZ = v.z;
		}

		min = { minX, 0.f, minZ };
		max = { maxX, 0.f, maxZ };
	}

	[[nodiscard]] constexpr static std::string_view getName() {
		return "TriangleOBB";
	}

	friend std::ostream& operator<<(std::ostream& os, const TriangleOBB& cmp);

	// AABB
	vec3  min {0};
	vec3  max {0};

	std::array<vec3, 3> vertices {};
};