#include <sys/SoundSystem.hpp>

#include <fmod.hpp>
#include <fmod_studio.hpp>
#include <fmod_errors.h>
#include <iostream>

#define ERRCHECK(_result) ERRCHECK_fn(_result, __FILE__, __LINE__)

void ERRCHECK_fn(const FMOD_RESULT res, const char * const file, const int line) {
	if (res != FMOD_OK)
	{
		std::cerr << file << "(Linea: " << line << "): " << res << " - " << FMOD_ErrorString(res) << std::endl;
	}
}

SoundSystem::~SoundSystem() {
	// TODO: generalizar sonidos

	for (const auto& item : sounds) {
		for (const auto& sound : item.second) {
			ERRCHECK( sound.instance->release() );
			ERRCHECK( sound.event->releaseAllInstances() );
		}
	}

	ERRCHECK( backingTrack.instance->release() );
	ERRCHECK( backingTrack.event->releaseAllInstances() );

	if(strings) strings->unload();
	if(master) master->unload();
	if(system) system->release();
	if(core) core->release();
}

void SoundSystem::init() {
	ERRCHECK ( FMOD::Studio::System::create(&system) );

	ERRCHECK ( system->getCoreSystem(&core) );
	ERRCHECK ( core->setSoftwareFormat(0, FMOD_SPEAKERMODE_DEFAULT, 0) );

	ERRCHECK ( system->initialize(1024, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, nullptr) );

	ERRCHECK ( system->loadBankFile(MASTER_BANK, FMOD_STUDIO_LOAD_BANK_NORMAL, &master) );
	ERRCHECK ( system->loadBankFile(MASTER_STRINGS_BANK, FMOD_STUDIO_LOAD_BANK_NORMAL, &strings) );

	{
		ERRCHECK ( system->getEvent(BACKGROUND_MUSIC_EVENT, &backingTrack.event) );
		ERRCHECK ( backingTrack.event->createInstance(&backingTrack.instance) );
		ERRCHECK ( backingTrack.instance->setVolume(0.25f) );
	}

	soundMessages.reserve(16);

	createSoundEvent(DEMON_SHOOT_EVENT, 16);
	createSoundEvent(ANGEL_SHOOT_EVENT, 16);
	createSoundEvent(ASSEMBLED_ATTACK_EVENT, 16);

	startBackgroundMusic();
}

void SoundSystem::update(const std::unique_ptr<GameContext>& context, const float deltaTime) {
	// TODO: generalizar (tipo entidad - ataque - switch - (dash) segun el modo) (FMOD Studio parametros)

	while (!soundMessages.empty()) {
		const auto& newSound = soundMessages.back();
		soundMessages.pop_back();

		FMOD_STUDIO_PLAYBACK_STATE state;

		for (auto & instance : soundEvents[newSound.soundEventName].instances) {
			instance->getPlaybackState(&state);

			if (state == FMOD_STUDIO_PLAYBACK_STOPPED) {
				instance->start();
				break;
			}
		}
	}

	ERRCHECK (system->update() );
}

void SoundSystem::reset() {
	// TODO: generalizar (cmp, vector, loqueseas)
	for (const auto& item : sounds)
		for (const auto& sound : item.second)
			ERRCHECK( sound.instance->stop(FMOD_STUDIO_STOP_IMMEDIATE) );

	ERRCHECK( backingTrack.instance->stop(FMOD_STUDIO_STOP_IMMEDIATE) );

	startBackgroundMusic();
}

void SoundSystem::startBackgroundMusic() {
	ERRCHECK( backingTrack.instance->start() );
}

void SoundSystem::createSoundEvent(const char * const name, const unsigned size) {
	soundEvents[name].instances = std::vector<Instance*>(size);

	ERRCHECK( system->getEvent(name, &soundEvents[name].event) ); // creo el evento
	for (unsigned i = 0; i < soundEvents[name].instances.capacity(); ++i) // recorro el vector de instancias hasta capacity
		ERRCHECK( soundEvents[name].event->createInstance(&soundEvents[name].instances[i]) ); // creo la instancia
}