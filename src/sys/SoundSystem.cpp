#include <sys/SoundSystem.hpp>

#define ERRCHECK(_result) ERRCHECK_fn(_result, __FILE__, __LINE__)

void ERRCHECK_fn(FMOD_RESULT res, const char* file, int line) {
	if (res != FMOD_OK)
	{
		std::cerr << file << "(Linea: " << line << "): " << res << " - " << FMOD_ErrorString(res) << std::endl;
		exit(-1);
	}
}

SoundSystem::~SoundSystem() {
    //Disparo demonio
	if(instanceDisparo_Demon) instanceDisparo_Demon->release();
	if(eventDisparo_Demon) eventDisparo_Demon->releaseAllInstances();
	//Musica ingame bucle
    if(instanceMusic_ingame) instanceMusic_ingame->release();
    if(eventMusic_ingame) eventMusic_ingame->releaseAllInstances();

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

	//Disparo demonio
	ERRCHECK ( system->getEvent(DEMON_SHOOT_EVENT, &eventDisparo_Demon) );
	ERRCHECK ( eventDisparo_Demon->createInstance(&instanceDisparo_Demon) );

    //Musica ingame bucle
    ERRCHECK ( system->getEvent(BACKGROUND_MUSIC_EVENT, &eventMusic_ingame) );
    ERRCHECK ( eventMusic_ingame->createInstance(&instanceMusic_ingame) );

	//Musica ingame bucle -> CAMBIAR A GAME CONTEXT DEL ESTADO DEL JUEGO (activo, pausado, menu principal, etc...)

	ERRCHECK ( instanceMusic_ingame->setVolume(0.25f) );
}

void SoundSystem::update(const std::unique_ptr<GameContext> &context, const float deltaTime) const {
	if(context->getPlayer().characterData->attacking)
	{
//		ERRCHECK ( instanceDisparo->setParameterByName("mode", context->getPlayer().characterData->mode) );
        //Disparo demonio
		ERRCHECK ( instanceDisparo_Demon->start() );
	}

//	TODO: Generalizar sonidos en bucle
	FMOD_STUDIO_PLAYBACK_STATE state;
	ERRCHECK( instanceMusic_ingame->getPlaybackState(&state) );
	if (state == FMOD_STUDIO_PLAYBACK_STOPPED)
		ERRCHECK ( instanceMusic_ingame->start() );
//	TODO: Generalizar sonidos en bucle

	ERRCHECK (system->update() );
}

void SoundSystem::reset() {
	ERRCHECK( instanceDisparo_Demon->stop(FMOD_STUDIO_STOP_IMMEDIATE) );
	ERRCHECK( instanceMusic_ingame->stop(FMOD_STUDIO_STOP_IMMEDIATE) );
}