#pragma once

#include <Engine/util/Vector3.hpp>

struct GameNode {
    explicit GameNode (
                          const Vector3f * const pos = nullptr,
                          const Vector3f * const rot = nullptr,
                          const Vector3f * const sca = nullptr
                      ) : position(pos),
                          rotation(rot),
                          scale(sca) { }

    virtual ~GameNode() = 0;

    // returns if node is alive
    explicit virtual operator bool() const = 0;

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
    virtual void setTexture(char *) const = 0;

    // sets if light should affect a node
    virtual void affectedByLight(bool) const = 0;

    protected :
    // we don't want to be able to modify the values from the pointer, but we may copy them
    // so these are non-const pointers to const values
        const Vector3f * position;
        const Vector3f * rotation;
        const Vector3f *    scale;
};