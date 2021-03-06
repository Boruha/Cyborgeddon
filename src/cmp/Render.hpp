#pragma once

#include <cmp/Component.hpp>
#include <Engine/EngineInterface/SceneInterface/INode.hpp>

struct Render : Component {

	explicit Render(const EntityType type, const EntityID ID,  vec3 * const p, vec3 * const r, vec3 * const s, const bool b)
		: Component(type, ID), needUpdate(b), pos(p), rot(r), sca(s) {  }

	[[nodiscard]] constexpr static std::string_view getName() {
		return "Render";
	}

	friend std::ostream& operator<<(std::ostream& os, const Render& phy);

	bool  needUpdate {  false  };

	INode   * node    { nullptr };
	unsigned  aniMode { 0 };

	// NO TOCAR EL ORDEN
	vec3  	* pos  	 { nullptr };
	vec3  	* rot	 { nullptr };
	vec3  	* sca	 { nullptr };
	// NO TOCAR EL ORDEN
};