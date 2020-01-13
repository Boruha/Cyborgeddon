#pragma once

#include <sys/System.hpp>

struct CharacterDataSystem : System {

	void init() override {  }
	void update(const std::unique_ptr<GameContext>& context, float deltaTime) const override;
};