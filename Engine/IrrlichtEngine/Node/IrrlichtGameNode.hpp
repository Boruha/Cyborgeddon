#pragma once

#include <Engine/EngineInterface/NodeInterface/GameNode.hpp>

namespace irr::scene {
    struct ISceneNode;
}

struct IrrlichtGameNode : GameNode {
    explicit IrrlichtGameNode (
            const Vector3f * const pos = nullptr,
            const Vector3f * const rot = nullptr,
            const Vector3f * const sca = nullptr
    ) : position(pos), rotation(rot), scale(sca) { }

    ~IrrlichtGameNode() override = 0;

    // returns if node is alive
    explicit operator bool() const final;

    void remove() override;

    // updates data immediately
    void update() const final;
    // updates using interpolation based on parameter
    void update(float) const final;

    // getters
    [[nodiscard]] const Vector3f & getPosition() const final;
    [[nodiscard]] const Vector3f & getRotation() const final;
    [[nodiscard]] const Vector3f & getScale   () const final;

    // setters
    void setPosition(const Vector3f &) const final;
    void setRotation(const Vector3f &) const final;
    void setScale   (const Vector3f &) const final;

    // setters using interpolation based on parameter
    void setPosition(float) const final;
    void setRotation(float) const final;
    void setScale   (float) const final;

    // set texture
    void setTexture(const char *) const final;

    // sets if light should affect a node
    void affectedByLight(bool) const final;

    protected :
        irr::scene::ISceneNode * node { nullptr };

    private :
        const Vector3f * position { nullptr };
        const Vector3f * rotation { nullptr };
        const Vector3f *    scale { nullptr };
};