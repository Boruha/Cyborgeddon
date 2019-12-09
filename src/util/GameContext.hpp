#pragma once

#include <vector>
#include <ent/Entity.hpp>

struct GameContext {
	GameContext() = default;

	virtual ~GameContext() = default;

	[[nodiscard]] virtual const std::vector<Entity>& getEntities() const = 0;
};