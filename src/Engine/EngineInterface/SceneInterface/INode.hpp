#pragma once

#include <glm/glm.hpp>

struct INode {
    virtual ~INode() = default;
    // returns if node is alive
    explicit virtual operator bool() const = 0;

    // removes node from scene
    virtual void remove() = 0;

    // updates data immediately
    virtual void update() = 0;
    // updates using interpolation based on parameter
    virtual void update(float) = 0;

    // getters
    [[nodiscard]] virtual const glm::vec3 & getPosition() const = 0;
    [[nodiscard]] virtual const glm::vec3 & getRotation() const = 0;
    [[nodiscard]] virtual const glm::vec3 & getScale   () const = 0;

    // setters
    virtual void setPosition(const glm::vec3 &) const = 0;
    virtual void setRotation(const glm::vec3 &) const = 0;
    virtual void setScale   (const glm::vec3 &) const = 0;

    // setters using interpolation based on parameter
    virtual void setPosition(float) const = 0;
    virtual void setRotation(float) const = 0;
    virtual void setScale   (float) const = 0;

    // set texture
    virtual void setTexture(const char *) const = 0;

    // sets if light should affect a node
    virtual void affectedByLight(bool) const = 0;
};