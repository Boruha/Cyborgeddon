#pragma once

#include <glm/glm.hpp>
#include <string_view>

using glm::vec3;

struct INode {
    virtual ~INode() = default;
    // returns if node is alive
    explicit virtual operator bool() const = 0;

    // removes node from scene
    virtual void remove() = 0;

    // getters
    [[nodiscard]] virtual const vec3 & getPosition() const = 0;
    [[nodiscard]] virtual const vec3 & getRotation() const = 0;
    [[nodiscard]] virtual const vec3 & getScale   () const = 0;
    [[nodiscard]] virtual const vec3 & getTarget()   const = 0;

	// setters
    virtual void setPosition(const vec3 &)  = 0;
    virtual void setRotation(const vec3 &)  = 0;
    virtual void setScale   (const vec3 &)  = 0;

	// setters
	virtual void setTarget(const vec3 &) = 0;

    // set texture
    virtual void setTexture(std::string_view) const = 0;

    // sets if light should affect a node
    virtual void affectedByLight(bool) const = 0;
};