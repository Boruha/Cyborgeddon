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

	ERRCHECK ( system->loadBankFile("./resources/sounds/banks/Master.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &master) );
	ERRCHECK ( system->loadBankFile("./resources/sounds/banks/Master.strings.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &strings) );

	//Disparo demonio
	ERRCHECK ( system->getEvent("event:/disparo_demon", &eventDisparo_Demon) );
	ERRCHECK ( eventDisparo_Demon->createInstance(&instanceDisparo_Demon) );

    //Musica ingame bucle
    ERRCHECK ( system->getEvent("event:/loop_music", &eventMusic_ingame) );
    ERRCHECK ( eventMusic_ingame->createInstance(&instanceMusic_ingame) );
}

void SoundSystem::update(const std::unique_ptr<GameContext> &context, const float deltaTime) const {
	if(context->getPlayer().characterData->attacking)
	{
//		ERRCHECK ( instanceDisparo->setParameterByName("mode", context->getPlayer().characterData->mode) );
        //Disparo demonio
		ERRCHECK ( instanceDisparo_Demon->start() );
        //Musica ingame bucle -> CAMBIAR A GAME CONTEXT DEL ESTADO DEL JUEGO (activo, pausado, menu principal, etc...)
        ERRCHECK ( instanceMusic_ingame->start() );
	}
	if()



	ERRCHECK (system->update() );
}