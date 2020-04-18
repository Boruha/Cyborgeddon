#pragma once

#include <sys/System.hpp>

#include <fmod_common.h>

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

template <>
struct std::hash<FMOD_GUID>
{
	size_t operator()(const FMOD_GUID & id) const
	{
		return (size_t(id.Data1) << 32u) + (size_t(id.Data2) << 16u) + (size_t(id.Data3) << 0u);
	}
};

template <>
struct std::equal_to<FMOD_GUID>
{
	bool operator()(const FMOD_GUID & id1, const FMOD_GUID & id2) const
	{
		size_t data4_1 {0}, data4_2 {0};

		for (unsigned i = 0; i < 8; ++i)
		{
			data4_1 |= size_t(id1.Data4[i] << i*8u);
			data4_2 |= size_t(id2.Data4[i] << i*8u);
		}

		return 		id1.Data1 == id2.Data1
				&&	id1.Data2 == id2.Data2
				&&	id1.Data3 == id2.Data3
				&& 	  data4_1 == data4_2;
	}
};

struct SoundSystem : System {
	~SoundSystem() override;

	void init() override;
	void fixedUpdate(const Context& context, float deltaTime) override;
	void update(const Context &context, float deltaTime) override {};
	void reset() override;

private:

	void startBackgroundMusic();

	void createMusicEvent(std::string_view, Music *, float = 1.f);

	void createSoundEvent(SoundParameter param);
    void createInstance(const Event *, Instance *&, float) const;

    FMOD::System * core 	{ nullptr };
	EngineSystem * system 	{ nullptr };
	Bank * master 			{ nullptr };
	Bank * strings 			{ nullptr };

    Music backingTrack;

	std::unordered_map<FMOD_GUID, Sound> soundEvents {};

	std::array<FMOD_GUID, NUM_MAX_PARAMETERS> eventID {};
};