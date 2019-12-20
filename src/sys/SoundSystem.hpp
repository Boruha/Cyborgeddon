#pragma once

#include <sys/System.hpp>
#include <fmod.hpp>
#include <fmod_studio.hpp>
#include <fmod_errors.h>

struct SoundSystem : public System {

	SoundSystem() = default;
	~SoundSystem() { if(system) system->release(); }

	void init() override;
	void update(const std::unique_ptr<GameContext>& context) const override;

private:
    FMOD::System* core { nullptr };
	FMOD::Studio::System* system { nullptr };
	FMOD::Studio::Bank* master { nullptr };
	FMOD::Studio::Bank* strings { nullptr };
	FMOD::Studio::EventDescription* eventDisparo { nullptr };
	FMOD::Studio::EventInstance* instanceDisparo { nullptr };
};