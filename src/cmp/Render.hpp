#pragma once

#include <cmp/Component.hpp>
#include <Engine/EngineInterface/SceneInterface/INode.hpp>

struct Render : Component {

	explicit Render(const EntityType type, const EntityID ID,  vec3 * const p, vec3 * const r, vec3 * const s, const bool b)
		: Component(type, ID), needUpdate(b), pos(p), rot(r), sca(s) {  }

	[[nodiscard]] std::string_view getName() const override {
		return "Render";
	}

	friend std::ostream& operator<<(std::ostream& os, const Render& phy);

	bool  needUpdate {  false  };

	INode   * node   { nullptr };

	// NO TOCAR EL ORDEN
	vec3  	* pos  	 { nullptr };
	vec3  	* rot	 { nullptr };
	vec3  	* sca	 { nullptr };
	// NO TOCAR EL ORDEN
};