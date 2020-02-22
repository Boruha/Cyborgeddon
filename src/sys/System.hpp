#pragma once

#include <util/GameContext.hpp>
#include <memory>
#include <util/Message.hpp>
#include <cmp/ComponentPool.hpp>

struct System {
	virtual void init() = 0;
	virtual void update(const std::unique_ptr<GameContext> &context, float deltaTime) = 0;
	virtual	void reset() = 0;

	protected :
		inline static std::vector<SoundMessage> 	  soundMessages;
	    inline static std::vector<DeathMessage> 	  deathMessages;
	    inline static std::vector<TriggerMessage>	triggerMessages;
};