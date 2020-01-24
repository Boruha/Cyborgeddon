#include <sys/SoundSystem.hpp>
#include <util/SoundPaths.hpp>

#define ERRCHECK(_result) ERRCHECK_fn(_result, __FILE__, __LINE__)

void ERRCHECK_fn(const FMOD_RESULT res, const char * const file, const int line) {
	if (res != FMOD_OK)
	{
		std::cerr << file << "(Linea: " << line << "): " << res << " - " << FMOD_ErrorString(res) << std::endl;
		exit(-1);
	}
}

SoundSystem::~SoundSystem() {
	//TODO: generalizar sonidos
	//Cambio demonio
	if(instanceCambioDemon) instanceCambioDemon->release();
	if(eventCambioDemon) eventCambioDemon->releaseAllInstances();
	//Cambio angel
	if(instanceCambioAngel) instanceCambioAngel->release();
	if(eventCambioAngel) eventCambioAngel->releaseAllInstances();
	//Disparo demonio
	if(instanceDisparoDemon) instanceDisparoDemon->release();
	if(eventDisparoDemon) eventDisparoDemon->releaseAllInstances();
    //Disparo angel
    if(instanceDisparoAngel) instanceDisparoAngel->release();
    if(eventDisparoAngel) eventDisparoAngel->releaseAllInstances();
	//Musica ingame bucle
    if(instanceMusicIngame) instanceMusicIngame->release();
    if(eventMusicIngame) eventMusicIngame->releaseAllInstances();

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
	ERRCHECK ( system->getEvent(DEMON_SHOOT_EVENT, &eventDisparoDemon) );
	ERRCHECK ( eventDisparoDemon->createInstance(&instanceDisparoDemon) );

    //Disparo angel
    ERRCHECK ( system->getEvent(ANGEL_SHOOT_EVENT, &eventDisparoAngel) );
    ERRCHECK ( eventDisparoAngel->createInstance(&instanceDisparoAngel) );

    //Cambio demonio
	ERRCHECK ( system->getEvent(DEMON_CHANGE_EVENT, &eventCambioDemon) );
	ERRCHECK ( eventCambioDemon->createInstance(&instanceCambioDemon) );

	//Cambio angel
	ERRCHECK ( system->getEvent(ANGEL_CHANGE_EVENT, &eventCambioAngel) );
	ERRCHECK ( eventCambioAngel->createInstance(&instanceCambioAngel) );

    //Musica ingame bucle
    ERRCHECK ( system->getEvent(BACKGROUND_MUSIC_EVENT, &eventMusicIngame) );
    ERRCHECK ( eventMusicIngame->createInstance(&instanceMusicIngame) );

	//Musica ingame bucle -> CAMBIAR A GAME CONTEXT DEL ESTADO DEL JUEGO (activo, pausado, menu principal, etc...)
	ERRCHECK ( instanceMusicIngame->setVolume(0.25f) );

	startBackgroundMusic();
}

void SoundSystem::update(const std::unique_ptr<GameContext>& context, const float deltaTime) const {
	auto& player = context->getPlayer();

	// TODO: generalizar (tipo entidad - ataque - switch - (dash) segun el modo) (FMOD Studio parametros)

    //Pegamos un tiro
	if (player.characterData->attacking) {
        if (player.characterData->mode == ANGEL) {
            ERRCHECK (instanceDisparoAngel->start());  //Disparo Angel
        } else {
            ERRCHECK (instanceDisparoDemon->start());  //Disparo Demonio
        }
    }
	//Cambiamos de modo
	if (player.characterData->switchingMode) {
		if (player.characterData->mode == ANGEL) {
			ERRCHECK (instanceCambioAngel->start());	  //Cambio Angel
		} else {
			ERRCHECK (instanceCambioDemon->start());  //Cambio Demonio
		}

		player.characterData->switchingMode = false; // TODO: quitar esto de aqui e intentar llamar al sistema/motor de audio en el momento en que se necesite
	}

	ERRCHECK (system->update() );
}

void SoundSystem::reset() {
	//TODO: generalizar (cmp, vector, loquesea)
	ERRCHECK( instanceCambioAngel->stop(FMOD_STUDIO_STOP_IMMEDIATE) );
	ERRCHECK( instanceCambioDemon->stop(FMOD_STUDIO_STOP_IMMEDIATE) );
	ERRCHECK( instanceDisparoDemon->stop(FMOD_STUDIO_STOP_IMMEDIATE) );
    ERRCHECK( instanceDisparoAngel->stop(FMOD_STUDIO_STOP_IMMEDIATE) );
	ERRCHECK( instanceMusicIngame->stop(FMOD_STUDIO_STOP_IMMEDIATE) );

	startBackgroundMusic();
}

void SoundSystem::startBackgroundMusic() {
	ERRCHECK( instanceMusicIngame->start() );
}