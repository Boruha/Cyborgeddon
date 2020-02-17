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
            irr::scene::ISceneManager * = nullptr,
            const vec3 *  = nullptr,
            const vec3 *  = nullptr,
            const vec3 *  = nullptr
    );

    ~IrrlichtObjectNode() override = default;

	explicit operator bool() const override;

	// removes node from scene
	void remove() override;

	// updates data immediately
	void update() override;
	// updates using interpolation based on parameter
	void update(float) override;

	// getters
	[[nodiscard]] const vec3 & getPosition() const override;
	[[nodiscard]] const vec3 & getRotation() const override;
	[[nodiscard]] const vec3 & getScale   () const override;

	// setters
	void setPosition(const vec3 &) const override;
	void setRotation(const vec3 &) const override;
	void setScale   (const vec3 &) const override;

	// setters using interpolation based on parameter
	void setPosition(float) const override;
	void setRotation(float) const override;
	void setScale   (float) const override;

	// set texture
	void setTexture(const char *) const override;

	// sets if light should affect a node
	void affectedByLight(bool) const override;

    void setMesh(const char *) override;

    protected :
        irr::scene::IMeshSceneNode * meshNode { nullptr };

    	std::unique_ptr<IrrlichtNodeImpl> p_impl { nullptr };
};