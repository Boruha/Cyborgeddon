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

	for (const auto& item : soundEvents)
		ERRCHECK( item.second.event->releaseAllInstances() );

	ERRCHECK( backingTrack.event->releaseAllInstances() );

	if(strings) strings->unload();
	if(master) master->unload();
	if(system) system->release();
	if(core) core->release();
}

void SoundSystem::init() {
	ERRCHECK ( EngineSystem::create(&system) );

	ERRCHECK ( system->getCoreSystem(&core) );
	ERRCHECK ( core->setSoftwareFormat(0, FMOD_SPEAKERMODE_DEFAULT, 0) );

	ERRCHECK ( system->initialize(1024, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, nullptr) );

	ERRCHECK ( system->loadBankFile(MASTER_BANK, FMOD_STUDIO_LOAD_BANK_NORMAL, &master) );
	ERRCHECK ( system->loadBankFile(MASTER_STRINGS_BANK, FMOD_STUDIO_LOAD_BANK_NORMAL, &strings) );

	soundMessages.reserve(16);

	createSoundEvent(DEMON_SHOOT_EVENT);        //Mensaje alojado en InputSystem.cpp -> void void InputSystem::update()
	createSoundEvent(ANGEL_SHOOT_EVENT);        //Mensaje alojado en InputSystem.cpp -> void void InputSystem::update()
	createSoundEvent(ASSEMBLED_ATTACK_EVENT);   //Mensaje alojado en ...
	createSoundEvent(DASH_PLAYER_EVENT);        //Mensaje alojado en InputSystem.cpp -> void InputSystem::shift_pressed()
	createSoundEvent(ANGEL_CHANGE_EVENT);       //Mensaje alojado en InputSystem.cpp -> void InputSystem::m_pressed()
	createSoundEvent(DEMON_CHANGE_EVENT);       //Mensaje alojado en InputSystem.cpp -> void InputSystem::m_pressed()
    createSoundEvent(PICKUP_KEY_EVENT);         //Mensaje alojado en CollisionSystem.cpp
    createSoundEvent(OPEN_DOOR_EVENT);          //Mensaje alojado en CollisionSystem.cpp
    createSoundEvent(DAMAGE_PLAYER_EVENT);      //Mensaje alojado en AttackSystem.cpp
    createSoundEvent(HITMARKER_EVENT);          //Mensaje alojado en HighSpeedCollisionSystem.cpp

	createMusicEvent(BACKGROUND_MUSIC_EVENT_4, &backingTrack, .2f);

	startBackgroundMusic();
}

void SoundSystem::update(const Context& context, const float deltaTime) {

	while (!soundMessages.empty()) {
		const auto& message = soundMessages.back(); // leo el mensaje

		FMOD_STUDIO_PLAYBACK_STATE state;   // me preparo para recibir un estado

		for (const auto & instance : soundEvents[message.soundEventName].instances) {   // recorro las instancias
			instance->getPlaybackState(&state);                                         // obtengo su estado

			if (state == FMOD_STUDIO_PLAYBACK_STOPPED) {                                // si no esta emitiendo ningun sonido
				instance->start();                                                      // la pongo a sonar
				break;                                                                  // corto el bucle
			}
		}

		soundMessages.pop_back();                                                       // elimino el mensaje del vector
	}

	ERRCHECK (system->update() );
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
	ERRCHECK( backingTrack.instance->start() );
}

void SoundSystem::createSoundEvent(const char * const name, const float volume) {
	ERRCHECK( system->getEvent(name, &soundEvents[name].event) ); // creo el Sound e inicializo su parametro event

	for (unsigned i = 0; i < soundEvents[name].instances.size(); ++i)
	    createInstance(soundEvents[name].event, soundEvents[name].instances[i], volume);
}

void SoundSystem::createMusicEvent(const char * const name, Music * music, const float volume) {
    ERRCHECK ( system->getEvent(name, &music->event) );
    createInstance(music->event, music->instance, volume);
}

void SoundSystem::createInstance(const Event * event, Instance *& instance, const float volume) const {
    ERRCHECK ( event->createInstance(&instance) );
    ERRCHECK ( instance->setVolume(volume) );
}