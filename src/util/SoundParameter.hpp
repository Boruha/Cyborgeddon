#pragma once

#include <util/SoundPaths.hpp>

enum SoundParameter {
	//PLAYER
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
	ACTION_OPEN_DOOR,
};

using sound_parameter_tuple = std::tuple<std::string_view, std::string_view, int>;

constexpr sound_parameter_tuple getSoundTuple(const SoundParameter p){
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

        case DAMAGE_PLAYER :            return {DAMAGE_EVENT, "mode", 1};
        case DASH_PLAYER :              return {DASH_PLAYER_EVENT,   "mode", 0};


            //ENEMY--> Segun codigo ahora mismo: Demon=0, Angel=1, Assembled=2
        case(ATTACK_ENEMY_ASSEMBLY):    return{ASSEMBLED_ATTACK_EVENT, "mode", 1};
        case(ACTION_ENEMY_HITMARKER):   return{DAMAGE_EVENT, "mode", 0};

            //ACTION SOUNDS
        case (ACTION_GET_KEY):          return{KEY_DOOR_EVENT, "mode", 0};
        case (ACTION_OPEN_DOOR):        return{KEY_DOOR_EVENT, "mode", 1};


        case ATTACK_ENEMY_DEMON:
            break;
        case ATTACK_ENEMY_ANGEL:
            break;

    }
}