#pragma once

#include <util/SoundPaths.hpp>

#include <tuple>

enum SoundParameter {
	NO_SOUND,
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

	//MENU SOUNDS
    MENU_CHANGE_OPTION,

    //SONGS SOUNDS
    SONG_1,
    SONG_2,
    SONG_3,
    SONG_4,

	//VIDEOS
	VIDEO_INTRO_GAME,
	VIDEO_CINEMATICA_1,
    VIDEO_CINEMATICA_2,
    VIDEO_TUTORIAL,


    NUM_MAX_PARAMETERS
};

using parameter_value = std::tuple<std::string_view, int>;

using event_parameter_value = std::tuple<std::string_view, parameter_value>;

constexpr parameter_value getParameterValue(const SoundParameter p) {
	switch(p) {
		//EN EL RETURN se devuelven 2 valores:
		// 1--> NOMBRE DEL NOMBRE PARAMETRO DEL EVENTO (Ir viendo en FMOD).
		// 2--> El valor del parametro. Hace que suene un sonido u otro del evento.

		case NO_SOUND :
			return { "", -1 };

		//PLAYER
		case ATTACK_PLAYER_DEMON :
			return { "mode", 0 };
		case ATTACK_PLAYER_ANGEL :
			return { "mode", 1 };

		case SWITCH_MODE_DEMON :
			return { "mode", 0 };
		case SWITCH_MODE_ANGEL :
			return { "mode", 1 };

		case DAMAGE_PLAYER :
			return { "mode", 1};
		case DASH_PLAYER :
			return { "mode", 0 };


			//ENEMY--> Segun codigo ahora mismo: Demon=0, Angel=1, Assembled=2
		case ATTACK_ENEMY_ASSEMBLY :
			return { "mode", 1 };
		case ACTION_ENEMY_HITMARKER :
			return { "mode", 0 };

			//ACTION SOUNDS
		case ACTION_GET_KEY :
			return { "mode", 0 };
		case ACTION_OPEN_DOOR :
			return { "mode", 1 };


		case ATTACK_ENEMY_DEMON :
			return { "", -1 };
		case ATTACK_ENEMY_ANGEL :
			return { "", -1 };


            //MENU SOUNDS
	    case MENU_CHANGE_OPTION :
	        return {"mode", 0};

            //SONGS SOUNDS
	    case SONG_1 :
	        return { "song", 0 };
        case SONG_2 :
            return { "song", 1 };
        case SONG_3 :
            return { "song", 2 };
        case SONG_4 :
            return { "song", 3 };

			//VIDEO SOUNDS
	    case VIDEO_INTRO_GAME :
            return { "song", 0 };
        case VIDEO_CINEMATICA_1 :
            return { "song", 1 };
        case VIDEO_CINEMATICA_2 :
            return { "song", 2 };
        case VIDEO_TUTORIAL :
            return { "song", 3 };



        case NUM_MAX_PARAMETERS :
            return { "", -1 };

	}

	return { "", -1 };
}

constexpr std::string_view getEvent(const SoundParameter p)
{
    switch(p)
    {
    	case NO_SOUND :
    		return "";

        //PLAYER
        case ATTACK_PLAYER_DEMON :
        	return { PLAYER_SHOOT_EVENT };
        case ATTACK_PLAYER_ANGEL :
        	return { PLAYER_SHOOT_EVENT };

        case SWITCH_MODE_DEMON :
        	return { CHANGE_MODE_EVENT 	};
        case SWITCH_MODE_ANGEL :
        	return { CHANGE_MODE_EVENT  };

        case DAMAGE_PLAYER :
        	return { DAMAGE_EVENT  		};
        case DASH_PLAYER :
        	return { DASH_PLAYER_EVENT  };


            //ENEMY--> Segun codigo ahora mismo: Demon=0, Angel=1, Assembled=2
        case ATTACK_ENEMY_ASSEMBLY :
        	return { ASSEMBLED_ATTACK_EVENT };
        case ACTION_ENEMY_HITMARKER :
        	return { DAMAGE_EVENT };


            //ACTION SOUNDS
        case ACTION_GET_KEY :
        	return { KEY_DOOR_EVENT };
        case ACTION_OPEN_DOOR :
        	return { KEY_DOOR_EVENT };


        case ATTACK_ENEMY_DEMON :
            return "";
        case ATTACK_ENEMY_ANGEL :
			return "";


			//MENU SOUNDS
        case  MENU_CHANGE_OPTION:
            return { MENU_SOUND_EVENT };


            //SONGS SOUNDS
        case SONG_1 :
            return { MAIN_MUSIC_EVENT };
        case SONG_2 :
            return { MAIN_MUSIC_EVENT };
        case SONG_3 :
            return { MAIN_MUSIC_EVENT };
        case SONG_4 :
            return { MAIN_MUSIC_EVENT };


			//VIDEO SOUNDS
        case VIDEO_INTRO_GAME :
            return { VIDEO_SOUND_EVENT };
        case VIDEO_CINEMATICA_1 :
            return { VIDEO_SOUND_EVENT };
        case VIDEO_CINEMATICA_2 :
           return { VIDEO_SOUND_EVENT };
        case VIDEO_TUTORIAL:
            return { VIDEO_SOUND_EVENT };


        case NUM_MAX_PARAMETERS :
            return "";

    }

    return "";
}
