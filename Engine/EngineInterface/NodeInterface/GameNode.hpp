#pragma once

#include <Engine/util/Vector3.hpp>

struct GameNode {
    virtual ~GameNode() = 0;
    // returns if node is alive
    explicit virtual operator bool() const = 0;

    // removes node from scene
    virtual void remove() = 0;

    // updates data immediately
    virtual void update() const = 0;
    // updates using interpolation based on parameter
    virtual void update(float) const = 0;

    // getters
    [[nodiscard]] virtual const Vector3f & getPosition() const = 0;
    [[nodiscard]] virtual const Vector3f & getRotation() const = 0;
    [[nodiscard]] virtual const Vector3f & getScale   () const = 0;

    // setters
    virtual void setPosition(const Vector3f &) const = 0;
    virtual void setRotation(const Vector3f &) const = 0;
    virtual void setScale   (const Vector3f &) const = 0;

    // setters using interpolation based on parameter
    virtual void setPosition(float) const = 0;
    virtual void setRotation(float) const = 0;
    virtual void setScale   (float) const = 0;

    // set texture
    virtual void setTexture(const char *) const = 0;

    // sets if light should affect a node
    virtual void affectedByLight(bool) const = 0;
};