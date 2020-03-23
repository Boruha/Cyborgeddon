#include <sys/SoundSystem.hpp>

#include <fmod.hpp>
#include <fmod_studio.hpp>
#include <fmod_errors.h>
#include <iostream>

#define ERRCHECK(_result) ERRCHECK_fn(_result, __FILE__, __LINE__)

void ERRCHECK_fn(const FMOD_RESULT res, const std::string_view file, const int line) {
	if (res != FMOD_OK)
	{
		std::cerr << file << "(Linea: " << line << "): " << res << " - " << FMOD_ErrorString(res) << "\n";
//		exit(-1);
	}
}

SoundSystem::~SoundSystem() {
	if (system)
		system->release();
}

void SoundSystem::init() {
	ERRCHECK ( EngineSystem::create(&system) );

	ERRCHECK ( system->getCoreSystem(&core) );
	ERRCHECK ( core->setSoftwareFormat(0, FMOD_SPEAKERMODE_DEFAULT, 0) );

	ERRCHECK ( system->initialize(16, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, nullptr) );

	ERRCHECK ( system->loadBankFile(MASTER_BANK.data(), FMOD_STUDIO_LOAD_BANK_NORMAL, &master) );
	ERRCHECK ( system->loadBankFile(MASTER_STRINGS_BANK.data(), FMOD_STUDIO_LOAD_BANK_NORMAL, &strings) );

	soundMessages.reserve(16);

	SoundParameter parameters [NUM_MAX_PARAMETERS]
	{
			ATTACK_PLAYER_DEMON,
			ATTACK_PLAYER_ANGEL,
			SWITCH_MODE_DEMON,      //El personaje pasa de angel a demonio
			SWITCH_MODE_ANGEL,      //El personaje pasa de demonio a angel
			DAMAGE_PLAYER,
			DASH_PLAYER,

			//ENEMY
			ATTACK_ENEMY_ASSEMBLY,
			ATTACK_ENEMY_DEMON,
			ATTACK_ENEMY_ANGEL,
			ACTION_ENEMY_HITMARKER,

			//ACTION SOUNDS
			ACTION_GET_KEY,
			ACTION_OPEN_DOOR
	};

	for (const auto & param : parameters)
		createSoundEvent(param);

	createMusicEvent(BACKGROUND_MUSIC_EVENT, &backingTrack, .2f);

	startBackgroundMusic();
}

void SoundSystem::update(const Context& context, const float deltaTime) {

	while (!soundMessages.empty()) {
		const auto parameter = soundMessages.back().parameter;

		const auto & [paramName, value] = getParameterValue(parameter);

		FMOD_STUDIO_PLAYBACK_STATE state;   // me preparo para recibir un estado

		for (const auto & instance : soundEvents[eventID[parameter]].instances) {   // recorro las instancias
		    instance->getPlaybackState(&state);                                         // obtengo su estado

			if (state == FMOD_STUDIO_PLAYBACK_STOPPED) { // si no esta emitiendo ningun sonido
			    instance->setParameterByName(paramName.data(), float(value));
				instance->start();                                                      // la pongo a sonar
				break;                                                                  // corto el bucle
			}
		}

		soundMessages.pop_back();                                                       // elimino el mensaje del vector
	}

	ERRCHECK ( system->update() );
}

void SoundSystem::reset() {
	// TODO: generalizar (cmp, vector, loqueseas)
	for (const auto& item : soundEvents)
		for (const auto& instance : item.second.instances)
			ERRCHECK( instance->stop(FMOD_STUDIO_STOP_IMMEDIATE) );

	ERRCHECK( backingTrack.instance->stop(FMOD_STUDIO_STOP_IMMEDIATE) );

	soundMessages.clear();
	soundMessages.reserve(16);

	startBackgroundMusic();
}

void SoundSystem::startBackgroundMusic() {
	ERRCHECK ( backingTrack.instance->start() );
	ERRCHECK (system->update() );
}

void SoundSystem::createSoundEvent(const SoundParameter param) {
	const std::string_view eventName = getEvent(param);

	if (eventName != "")
	{
		Sound sound { };

		ERRCHECK( system->getEvent(eventName.data(), &sound.event) ); // creo el Sound e inicializo su parametro event

		auto & idEvent = eventID[param];

		ERRCHECK( sound.event->getID(&idEvent) );

		auto it = soundEvents.find(idEvent);

		if (it == soundEvents.end())
		{
			soundEvents[idEvent] = sound;

			for (unsigned i = 0; i < soundEvents[idEvent].instances.size(); ++i)
				createInstance(soundEvents[idEvent].event, soundEvents[idEvent].instances[i], 1.f);
		}
	}

}

void SoundSystem::createMusicEvent(const std::string_view name, Music * music, const float volume) {
    ERRCHECK ( system->getEvent(name.data(), &music->event) );
    createInstance(music->event, music->instance, volume);
}

void SoundSystem::createInstance(const Event * event, Instance *& instance, const float volume) const {
    ERRCHECK ( event->createInstance(&instance) );
    ERRCHECK ( instance->setVolume(volume) );
}
