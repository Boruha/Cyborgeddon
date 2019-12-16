#pragma once

#include <src/util/GameContext.hpp>
#include <memory>

struct System {
	virtual void init() = 0;
	virtual void update(const std::unique_ptr<GameContext>& context) const = 0;
};