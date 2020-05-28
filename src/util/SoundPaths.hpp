#pragma once

// SOUND BANKS
constexpr std::string_view MASTER_BANK				= "../resources/sounds/banks/Master.bank";
constexpr std::string_view MASTER_STRINGS_BANK 		= "../resources/sounds/banks/Master.strings.bank";

// SOUND EVENTS

//PLAYER
constexpr std::string_view PLAYER_SHOOT_EVENT       = "event:/player/disparo_player";
constexpr std::string_view CHANGE_MODE_EVENT		= "event:/player/change_mode";
constexpr std::string_view DASH_PLAYER_EVENT        = "event:/player/dash_player";
constexpr std::string_view PLAYER_CANT_USE          = "event:/player/cant_use";

//MUSIC
constexpr std::string_view MAIN_MUSIC_EVENT 	    = "event:/music/main_music";

//ENEMY
constexpr std::string_view ASSEMBLED_ATTACK_EVENT   = "event:/enemies/assembled";
constexpr std::string_view DEMON_ATTACK_EVENT       = "event:/enemies/demon";
constexpr std::string_view ANGEL_ATTACK_EVENT       = "event:/enemies/angel";


//OBJECTS
constexpr std::string_view KEY_DOOR_EVENT           = "event:/objects/action_key_door";
constexpr std::string_view DAMAGE_EVENT             = "event:/objects/damage";

//MENU
constexpr std::string_view MENU_SOUND_EVENT         = "event:/menu/menu_sounds";

//VIDEOS
// parametro 0 -> Video de cuando empieza el juego (Sunlight Logo + Animacion Cyborgeddon) (Musica + Voces)
// parametro 1 -> Video de la primera cinematica (Cuando le damos a "Play", justo antes de empezar a jugar) (Musica +
// parametro 2 -> Video de la segunda cinemática (Cuando completamos el juego, Video Final + Creditos) (Musica + Voces)
// parametro 3 -> Video del tutorial de Cyborgeddon (Musica + voces)
constexpr std::string_view VIDEO_SOUND_EVENT       = "event:/video/video_sound";






