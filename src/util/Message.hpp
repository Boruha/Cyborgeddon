#pragma once

#include <util/SoundPaths.hpp>

enum Parameter {
    //PLAYER
    ATTACK_PLAYER_DEMON,
    ATTACK_PLAYER_ANGEL,
    SWITCH_MODE_DEMON,      //El personaje pasa de angel a demonio
    SWITCH_MODE_ANGEL,      //EL personaje pasa de demonio a angel
    DAMAGE_PLAYER,
    DASH_PLAYER,

    //ENEMY
    ATTACK_ENEMY_ASSEMBLY,
    ATTACK_ENEMY_DEMON,
    ATTACK_ENEMY_ANGEL,
    ACTION_ENEMY_HITMARKER,

    //ACTION SOUNDS
    ACTION_GET_KEY,
    ACTION_OPEN_DOOR,

};

using sound_tuple = std::tuple<std::string_view, std::string_view, int>;

constexpr sound_tuple getSoundTuple(const Parameter p){
    switch(p){
        //EN EL RETURN se devuelven 3 valores:
            // 1--> NOMBRE DEL EVENTO REAL (DECLARADO EN soundpaths.hpp y soundsystem.cpp)
            // 2--> NOMBRE DEL NOMBRE PARAMETRO DEL EVENTO (Ir viendo en FMOD).
            // 3--> El valor del parametro. Hace que suene un sonido u otro del evento.

        //PLAYER
        case ATTACK_PLAYER_DEMON :      return {PLAYER_SHOOT_EVENT, "mode", 0};
        case ATTACK_PLAYER_ANGEL :      return {PLAYER_SHOOT_EVENT, "mode", 1};

        case SWITCH_MODE_DEMON :        return {CHANGE_MODE_EVENT,  "mode", 0};
        case SWITCH_MODE_ANGEL :        return {CHANGE_MODE_EVENT,  "mode", 1};

        case DAMAGE_PLAYER :            return {DAMAGE_PLAYER_EVENT, "mode", 0};
        case DASH_PLAYER :              return {DASH_PLAYER_EVENT,   "mode", 0};


        //ENEMY
        //Segun codigo ahora mismo: Demon=0, Angel=1, Assembled=2
        case(ATTACK_ENEMY_ASSEMBLY):    return{ASSEMBLED_ATTACK_EVENT, "mode", 0};
        // case(ATTACK_ENEMY_DEMON):       return{"enemy", 0};
        // case(ATTACK_ENEMY_ANGEL):       return{"enemy", 1};
         case(ACTION_ENEMY_HITMARKER):   return{HITMARKER_EVENT, "mode", 0};

        //ACTION SOUNDS
        case (ACTION_GET_KEY):          return{PICKUP_KEY_EVENT, "mode", 0};
        case (ACTION_OPEN_DOOR):        return{OPEN_DOOR_EVENT, "mode", 0};


        case ATTACK_ENEMY_DEMON:
            break;
        case ATTACK_ENEMY_ANGEL:
            break;

    }
}

struct SoundMessage { //ESTE SERA EL DEFINITIVO
	explicit SoundMessage(const Parameter param)
            : tuple(getSoundTuple(param)) { }

    const sound_tuple tuple;
};

struct DeathMessage {
    explicit DeathMessage(const EntityID _ID)
        : ID(_ID) { }

    const EntityID ID {0};
};

struct TriggerMessage {
	explicit TriggerMessage(const EntityType _type1, const EntityID _ID1, const EntityType _type2, const EntityID _ID2)
		: type1(_type1), ID1(_ID1), type2(_type2), ID2(_ID2) { }

	const EntityType type1 { UNDEFINED };
	const EntityID ID1 { 0 };

	const EntityType type2 { UNDEFINED };
	const EntityID ID2 { 0 };
};

struct DamageMessage {
	explicit DamageMessage(const EntityID _ID, const float dmg) : ID(_ID), damage(dmg) { }

	const EntityID ID  {0};
	const float damage {0};
};