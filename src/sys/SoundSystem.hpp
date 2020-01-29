#pragma once

#include <sys/System.hpp>

#include <util/SoundPaths.hpp>

#include <unordered_map>

namespace FMOD {
    namespace Studio {
        struct EventDescription;
        struct EventInstance;
        struct System;
        struct Bank;
    }

    struct System;
}

using Event = FMOD::Studio::EventDescription;
using Instance = FMOD::Studio::EventInstance;
using EngineSystem = FMOD::Studio::System;

struct Sound {
	Event* 		event 		{  nullptr  };
	Instance* 	instance 	{  nullptr  };
	ModeType	mode		{  NEUTRAL  };
};

struct SoundSystem : public System {

	SoundSystem() = default;
	~SoundSystem();

	void init() override;
	void update(const std::unique_ptr<GameContext>& context, float deltaTime) const override;
	void reset() override;

	void startBackgroundMusic();

private:

    FMOD::System* core { nullptr };
	FMOD::Studio::System* system { nullptr };
	FMOD::Studio::Bank* master { nullptr };
	FMOD::Studio::Bank* strings { nullptr };


    // TODO : crear enum como clave de este mapa
    Sound backingTrack;
	std::unordered_map<bool, std::vector<Sound>> sounds; // Ahora mismo key true = attacking, key false = changing

	const char * const attackEventName[2] {
		DEMON_SHOOT_EVENT,
		ANGEL_SHOOT_EVENT
	};

	const char * const changeEventName[2] {
		DEMON_CHANGE_EVENT,
		ANGEL_CHANGE_EVENT
	};
};