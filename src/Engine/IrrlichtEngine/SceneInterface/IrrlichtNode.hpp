#pragma once

#include <Engine/EngineInterface/SceneInterface/Node.hpp>

namespace irr::scene {
    struct ISceneNode;
}

struct IrrlichtNode : virtual Node {
    explicit IrrlichtNode (
            const glm::vec3 * const pos = nullptr,
            const glm::vec3 * const rot = nullptr,
            const glm::vec3 * const sca = nullptr
    ) : position(pos), rotation(rot), scale(sca) { }

    ~IrrlichtNode() override = default;

    // returns if node is alive
    explicit operator bool() const final;

    void remove() override;

    // updates data immediately
    void update() override;
    // updates using interpolation based on parameter
    void update(float) override;

    // getters
    [[nodiscard]] const glm::vec3 & getPosition() const final;
    [[nodiscard]] const glm::vec3 & getRotation() const final;
    [[nodiscard]] const glm::vec3 & getScale   () const final;

    // setters
    void setPosition(const glm::vec3 &) const final;
    void setRotation(const glm::vec3 &) const final;
    void setScale   (const glm::vec3 &) const final;

    // setters using interpolation based on parameter
    void setPosition(float) const final;
    void setRotation(float) const final;
    void setScale   (float) const final;

    // set texture
    void setTexture(const char *) const final;

    // sets if light should affect a node
    void affectedByLight(bool) const final;

    protected :
        irr::scene::ISceneNode * node   { nullptr };

        const glm::vec3 * position      { nullptr };
        const glm::vec3 * rotation      { nullptr };
        const glm::vec3 *    scale      { nullptr };
};