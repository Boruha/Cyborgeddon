#pragma once

#include <sys/System.hpp>
#include <fmod.hpp>
#include <fmod_studio.hpp>
#include <fmod_errors.h>

struct SoundSystem : public System {

	SoundSystem() = default;
	~SoundSystem();

	void init() override;
	void update(const std::unique_ptr<GameContext> &context, float deltaTime) const override;
	void reset() override;

private:
    FMOD::System* core { nullptr };
	FMOD::Studio::System* system { nullptr };
	FMOD::Studio::Bank* master { nullptr };
	FMOD::Studio::Bank* strings { nullptr };
	//Disparo demonio
	FMOD::Studio::EventDescription* eventDisparo_Demon { nullptr };
	FMOD::Studio::EventInstance* instanceDisparo_Demon { nullptr };
	//Musica ingame bucle
    FMOD::Studio::EventDescription* eventMusic_ingame { nullptr };
    FMOD::Studio::EventInstance* instanceMusic_ingame { nullptr };
};