#pragma once

#include <Engine/EngineInterface/SceneInterface/IObjectNode.hpp>

#include <Engine/IrrlichtEngine/SceneInterface/IrrlichtNodeImpl.hpp>

#include <memory>

namespace irr::scene {
	struct ISceneManager;
	struct IMeshSceneNode;
}

struct IrrlichtObjectNode : IObjectNode {
    explicit IrrlichtObjectNode (
            irr::scene::ISceneManager * = nullptr
    );

    ~IrrlichtObjectNode() override = default;

	explicit operator bool() const override;

	// removes node from scene
	void remove() override;

	// getters
	[[nodiscard]] const vec3 & getPosition() const override;
	[[nodiscard]] const vec3 & getRotation() const override;
	[[nodiscard]] const vec3 & getScale   () const override;
	[[nodiscard]] const vec3 & getTarget  () const override {
		assert(false);
		// jamas de los jamases deberia llegar aqui
		return *(new vec3());
	}


	// setters
	void setPosition(const vec3 &) override;
	void setRotation(const vec3 &) override;
	void setScale   (const vec3 &) override;
	void setTarget	(const vec3 &) override { }

	// set texture
	void setTexture(std::string_view) const override;

	// sets if light should affect a node
	void affectedByLight(bool) const override;

    void setMesh(std::string_view) override;

    protected :
        irr::scene::IMeshSceneNode * meshNode { nullptr };

    	std::unique_ptr<IrrlichtNodeImpl> p_impl { nullptr };
};