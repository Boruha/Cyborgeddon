#pragma once

#include <cmp/Component.hpp>

struct INode;

struct Render : Component {

	explicit Render(const EntityType type, const EntityID ID,  vec3 * const p, vec3 * const r, vec3 * const s)
		: Component(type, ID), pos(p), rot(r), sca(s) {  }

	[[nodiscard]] std::string_view getName() const override {
		return "Render";
	}

	friend std::ostream& operator<<(std::ostream& os, const Render& phy);

	INode	* node;

	vec3  	* pos  	{ nullptr };
	vec3  	* rot	{ nullptr };
	vec3  	* sca	{ nullptr };
};