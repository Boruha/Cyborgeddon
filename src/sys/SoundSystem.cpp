#include <sys/SoundSystem.hpp>

#include <fmod.hpp>
#include <fmod_studio.hpp>
#include <fmod_errors.h>

#define ERRCHECK(_result) ERRCHECK_fn(_result, __FILE__, __LINE__)

void ERRCHECK_fn(const FMOD_RESULT res, const char * const file, const int line) {
	if (res != FMOD_OK)
	{
		std::cerr << file << "(Linea: " << line << "): " << res << " - " << FMOD_ErrorString(res) << std::endl;
	}
}

SoundSystem::~SoundSystem() {
	// TODO: generalizar sonidos

	for (auto& item : sounds) {
		for (auto& sound : item.second) {
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

	sounds[true].reserve(2);		// eventos de ataque
	sounds[false].reserve(2);	// eventos de cambio

	for (unsigned i = 0; i < 2; ++i) {
		sounds[true].emplace_back();
		ERRCHECK ( system->getEvent(attackEventName[i], &sounds[true][i].event) );
		ERRCHECK ( sounds[true][i].event->createInstance(&sounds[true][i].instance) );

		sounds[false].emplace_back();
		ERRCHECK ( system->getEvent(changeEventName[i], &sounds[false][i].event) );
		ERRCHECK ( sounds[false][i].event->createInstance(&sounds[false][i].instance) );
	}

	startBackgroundMusic();
}

void SoundSystem::update(const std::unique_ptr<GameContext>& context, const float deltaTime) const {
	// TODO: generalizar (tipo entidad - ataque - switch - (dash) segun el modo) (FMOD Studio parametros)

	for (auto& data : std::get<vector<CharacterData>>(context->getComponents(CHARACTER_DATA_TYPE))) {
		if (data.mode == DEMON) {
			if (data.attacking){
				const_cast<std::unordered_map<bool, std::vector<Sound>>&>(sounds)[true][0].instance->start();
				data.attacking = false;
			}
			if (data.switchingMode) {
				const_cast<std::unordered_map<bool, std::vector<Sound>>&>(sounds)[false][0].instance->start();
				data.switchingMode = false;
			}
		} else if (data.mode == ANGEL) {
			if (data.attacking) {
				const_cast<std::unordered_map<bool, std::vector<Sound>>&>(sounds)[true][1].instance->start();
				data.attacking = false;
			}
			if (data.switchingMode) {
				const_cast<std::unordered_map<bool, std::vector<Sound>>&>(sounds)[false][1].instance->start();
				data.switchingMode = false;
			}
		}

		// TODO: quitar esto de aqui e intentar llamar al sistema/motor de audio en el momento en que se necesite
		data.attacking = false;
		data.switchingMode = false;
		data.dashing = false;
	}

	ERRCHECK (system->update() );
}

void SoundSystem::reset() {
	// TODO: generalizar (cmp, vector, loquesea)
	for (auto& item : sounds)
		for (auto& sound : item.second)
			ERRCHECK( sound.instance->stop(FMOD_STUDIO_STOP_IMMEDIATE) );

	ERRCHECK( backingTrack.instance->stop(FMOD_STUDIO_STOP_IMMEDIATE) );

	startBackgroundMusic();
}

void SoundSystem::startBackgroundMusic() {
	ERRCHECK( backingTrack.instance->start() );
}