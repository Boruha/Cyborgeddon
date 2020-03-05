#pragma once

#include <Engine/EngineInterface/SceneInterface/INode.hpp>

namespace irr::scene {
	struct ISceneNode;
}

struct IrrlichtNodeImpl : INode {
	explicit IrrlichtNodeImpl(irr::scene::ISceneNode *);
	~IrrlichtNodeImpl() override = default;

    explicit operator bool() const override;

	void remove() override;

	[[nodiscard]] const vec3 & getPosition() const override;
	[[nodiscard]] const vec3 & getRotation() const override;
	[[nodiscard]] const vec3 & getScale   () const override;
	[[nodiscard]] const vec3 & getTarget  () const override {
		assert(false);
		// jamas de los jamases deberia llegar aqui
		return *(new vec3());
	}

	void setPosition(const vec3 &) override;
	void setRotation(const vec3 &) override;
	void setScale   (const vec3 &) override;
	void setTarget	(const vec3 &) override { }

	void setTexture(std::string_view) const override;

	void affectedByLight(bool) const override;

	private :

		irr::scene::ISceneNode * node { nullptr };

		vec3 pos {0};
		vec3 rot {0};
		vec3 sca {0};
};