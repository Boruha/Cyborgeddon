#pragma once

#include <sys/System.hpp>

#include <util/SoundPaths.hpp>

#include <unordered_map>

namespace FMOD {
    namespace Studio {
        struct EventDescription;
        struct EventInstance;
        struct System;
        struct Bank;
    }

    struct System;
}

using Event = FMOD::Studio::EventDescription;
using Instance = FMOD::Studio::EventInstance;
using EngineSystem = FMOD::Studio::System;
using Bank = FMOD::Studio::Bank;

struct Sound {
	Event* event { nullptr };
	std::array<Instance*, 16> instances;
};

struct Music {
    Event * event { nullptr };
    Instance * instance { nullptr };
};

struct SoundSystem : public System {

	SoundSystem() = default;
	~SoundSystem();

	void init() override;
	void update(const std::unique_ptr<GameContext>& context, float deltaTime) override;
	void reset() override;

private:

<<<<<<< HEAD
    FMOD::System* core { nullptr };
	FMOD::Studio::System* system { nullptr };
	FMOD::Studio::Bank* master { nullptr };
	FMOD::Studio::Bank* strings { nullptr };

	                  //PERSONAJE           //ENSAMBLADO    //DEMONIOS   //ANGELES
	enum TipoSonido { ATTACK, CHANGE, DASH , ENEMY_ASSEMBLED, ENEMY_DEMON, ENEMY_ANGEL};
=======
	void startBackgroundMusic();

	void createSoundEvent(const char *, float = 1.f);
	void createMusicEvent(const char *, Music *, float = 1.f);
>>>>>>> master

    void createInstance(const Event *, Instance *&, float) const;

<<<<<<< HEAD
	//PERSONAJE
	const char * const attackEventName[2] {
		DEMON_SHOOT_EVENT,
		ANGEL_SHOOT_EVENT
	};
=======
    FMOD::System* core { nullptr };
	EngineSystem * system { nullptr };
	Bank * master { nullptr };
	Bank * strings { nullptr };
>>>>>>> master

    Music backingTrack;

<<<<<<< HEAD
	const char * const dashEventName[2]{
	    DASH_PLAYER_EVENT,
	    DASH_PLAYER_EVENT
	};

	//ENEMIGO ASSEMBLED
    const char * const NeutralAttackEventName[2]{
        ASSEMBLED_ATTACK_EVENT,
        ASSEMBLED_ATTACK_EVENT
    };

	//ENEMIGO DEMONIO

	//ENEMIGO ANGEL
=======
	std::unordered_map<const char *, Sound> soundEvents;
>>>>>>> master
};