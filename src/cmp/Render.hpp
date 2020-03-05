#pragma once

#include <cmp/Component.hpp>
#include <Engine/EngineInterface/SceneInterface/INode.hpp>

struct Render : Component {

	explicit Render(const EntityType type, const EntityID ID,  vec3 * const p, vec3 * const r, vec3 * const s, const bool b)
		: Component(type, ID), needUpdate(b), pos(p), rot(r), sca(s) {  }

	// constructor de copia + operador asignacion por copia son necesarios porque tenemos un unique ptr (el cual no es copiable)

	// no se dara el caso de que copiemos un unique ptr con un valor dentro (a menos que la liemos parda jajaja) pero de todas
	// formas tenemos que hacerlos

	Render(const Render& r) noexcept : Component(r.entityType, r.entityID) {
		needUpdate = r.needUpdate;

		node = std::move(const_cast<Render &>(r).node);

		pos = r.pos;
		rot = r.rot;
		sca = r.sca;
	};

	Render& operator=(const Render& r) noexcept {
		if (&r == this)
			return *this;

		entityID = r.entityID;
		entityType = r.entityType;

		needUpdate = r.needUpdate;

		node = std::move(const_cast<Render &>(r).node);

		pos = r.pos;
		rot = r.rot;
		sca = r.sca;

		return *this;
	}

	[[nodiscard]] std::string_view getName() const override {
		return "Render";
	}

	friend std::ostream& operator<<(std::ostream& os, const Render& phy);

	bool    needUpdate { false };

	std::unique_ptr<INode> node { nullptr };

	// NO TOCAR EL ORDEN
	vec3  	* pos  	 { nullptr };
	vec3  	* rot	 { nullptr };
	vec3  	* sca	 { nullptr };
	// NO TOCAR EL ORDEN
};