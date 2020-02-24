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
using Bank = FMOD::Studio::Bank;

struct Sound {
	Event* event { nullptr };
	std::array<Instance*, 16> instances;
};

struct Music {
    Event * event { nullptr };
    Instance * instance { nullptr };
};

struct SoundSystem : System {
	~SoundSystem();

	void init() override;
	void update(const Context& context, float deltaTime) override;
	void reset() override;

private:

	void startBackgroundMusic();

	void createSoundEvent(const char *, float = 1.f);
	void createMusicEvent(const char *, Music *, float = 1.f);

    void createInstance(const Event *, Instance *&, float) const;

    FMOD::System* core { nullptr };
	EngineSystem * system { nullptr };
	Bank * master { nullptr };
	Bank * strings { nullptr };

    Music backingTrack;

	std::unordered_map<const char *, Sound> soundEvents;
};