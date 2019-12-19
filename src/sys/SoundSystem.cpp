#include <sys/SoundSystem.hpp>

#define ERRCHECK(_result) ERRCHECK_fn(_result, __FILE__, __LINE__)

void ERRCHECK_fn(FMOD_RESULT res, const char* file, int line) {
	if (res != FMOD_OK)
	{
		std::cerr << file << "(Linea: " << line << "): " << res << " - " << FMOD_ErrorString(res) << std::endl;
		exit(-1);
	}
}

void SoundSystem::init() {
	ERRCHECK ( FMOD::Studio::System::create(&system) );

	ERRCHECK ( system->initialize(1024, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, nullptr) );
/*
	ERRCHECK ( system->getCoreSystem(&core) );
	ERRCHECK ( core->setSoftwareFormat(0, FMOD_SPEAKERMODE_MONO, 0) );
*/
	ERRCHECK ( system->loadBankFile("./resources/sounds/banks/Master.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &master) );
	ERRCHECK ( system->loadBankFile("./resources/sounds/banks/Master.strings.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &strings) );

	ERRCHECK ( system->getEvent("event:/Disparos", &eventDisparo) );

	ERRCHECK ( eventDisparo->createInstance(&instanceDisparo) );
}

void SoundSystem::update(const std::unique_ptr<GameContext>& context) const {
	if(context->getPlayer()->shooting)
	{
	//	ERRCHECK ( instanceDisparo->setParameterByName("mode", context->getPlayer()->mode) );
		ERRCHECK ( instanceDisparo->start() );
		std::cout << "Disparo!" << std::endl;
	}

	ERRCHECK (system->update() );
}