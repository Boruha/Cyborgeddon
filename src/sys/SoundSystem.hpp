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

	void startBackgroundMusic();

private:
    FMOD::System* core { nullptr };
	FMOD::Studio::System* system { nullptr };
	FMOD::Studio::Bank* master { nullptr };
	FMOD::Studio::Bank* strings { nullptr };
	//Disparo demonio
	FMOD::Studio::EventDescription* eventDisparoDemon { nullptr };
	FMOD::Studio::EventInstance* instanceDisparoDemon { nullptr };
	//Disparo angel
    FMOD::Studio::EventDescription* eventDisparoAngel { nullptr };
    FMOD::Studio::EventInstance* instanceDisparoAngel { nullptr };
	//Cambio demonio
	FMOD::Studio::EventDescription* eventCambioDemon  { nullptr };
	FMOD::Studio::EventInstance* instanceCambioDemon  { nullptr };
	//Cambio angel
	FMOD::Studio::EventDescription* eventCambioAngel  { nullptr };
	FMOD::Studio::EventInstance* instanceCambioAngel  { nullptr };
	//Musica ingame bucle
    FMOD::Studio::EventDescription* eventMusicIngame  { nullptr };
    FMOD::Studio::EventInstance* instanceMusicIngame  { nullptr };
};