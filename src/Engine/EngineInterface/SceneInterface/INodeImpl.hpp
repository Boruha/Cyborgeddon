#pragma once

#include <glm/glm.hpp>

using glm::vec3;

struct INodeImpl {
	virtual ~INodeImpl() = default;

	virtual void remove() = 0;

	virtual void update() = 0;
	virtual void update(float) = 0;

	[[nodiscard]] virtual const vec3 & getPosition() const = 0;
	[[nodiscard]] virtual const vec3 & getRotation() const = 0;
	[[nodiscard]] virtual const vec3 & getScale   () const = 0;

	virtual void setPosition(const vec3 &) const = 0;
	virtual void setRotation(const vec3 &) const = 0;
	virtual void setScale   (const vec3 &) const = 0;

	virtual void setPosition(float) const = 0;
	virtual void setRotation(float) const = 0;
	virtual void setScale   (float) const = 0;

	virtual void setTexture(const char *) const = 0;

	virtual void affectedByLight(bool) const = 0;
};