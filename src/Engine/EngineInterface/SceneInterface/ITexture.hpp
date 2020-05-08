#pragma once

#include <glm/glm.hpp>

struct ITexture {
	virtual ~ITexture() = default;

	virtual void render() const = 0;

	virtual void setPosition(int, int) = 0;

	[[maybe_unused]] virtual void setWidth(unsigned) = 0;
	[[maybe_unused]] virtual void setHeight(unsigned) = 0;

	[[maybe_unused]] virtual void setSize(unsigned, unsigned) = 0;

	[[nodiscard]] virtual glm::vec2 getSize() const = 0;
};