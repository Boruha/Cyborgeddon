#pragma once

#include <vector>
#include <ent/Entity.hpp>

struct GameContext {
	GameContext() = default;

	virtual ~GameContext() = default;

	[[nodiscard]] virtual const std::vector<std::unique_ptr<EntityPlayer>>& getPlayers() const = 0;
	[[nodiscard]] virtual const std::vector<std::unique_ptr<EntityCamera>>& getCameras() const = 0;
};