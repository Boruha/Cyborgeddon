#pragma once

#include <Engine/EngineInterface/SceneInterface/INodeImpl.hpp>

namespace irr::scene {
	struct ISceneNode;
}

struct IrrlichtNodeImpl : INodeImpl {
	explicit IrrlichtNodeImpl(irr::scene::ISceneNode *, const vec3 *, const vec3 *, const vec3 *);
	~IrrlichtNodeImpl() override = default;

	void remove() override;

	void update() override;
	void update(float delta) override;

	[[nodiscard]] const glm::vec3 & getPosition() const override;
	[[nodiscard]] const glm::vec3 & getRotation() const override;
	[[nodiscard]] const glm::vec3 & getScale   () const override;

	void setPosition(const glm::vec3 &) const override;
	void setRotation(const glm::vec3 &) const override;
	void setScale   (const glm::vec3 &) const override;

	void setPosition(float delta) const override;
	void setRotation(float delta) const override;
	void setScale   (float delta) const override;

	void setTexture(const char *) const override;

	void affectedByLight(bool) const override;

	private :

		irr::scene::ISceneNode * node { nullptr };

		const vec3 * const position { nullptr };
		const vec3 * const rotation { nullptr };
		const vec3 * const    scale { nullptr };
};