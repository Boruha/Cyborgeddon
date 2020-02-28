#pragma once

#include <cmp/Component.hpp>

struct INode;

struct Render : Component {

	explicit Render(const EntityType type, const EntityID ID,  vec3 * const p, vec3 * const r, vec3 * const s)
		: Component(type, ID), pos(p), rot(r), sca(s) {  }

	INode	* node;

	vec3  	* pos  	{ nullptr };
	vec3  	* rot	{ nullptr };
	vec3  	* sca	{ nullptr };
};