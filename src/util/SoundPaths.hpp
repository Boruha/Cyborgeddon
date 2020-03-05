#pragma once
// SOUND BANKS
constexpr std::string_view MASTER_BANK				= "./resources/sounds/banks/Master.bank";
constexpr std::string_view MASTER_STRINGS_BANK 		= "./resources/sounds/banks/Master.strings.bank";



// SOUND EVENTS
//PLAYER
constexpr std::string_view PLAYER_SHOOT_EVENT       = "event:/player/disparo_player"; //prueba
//constexpr std::string_view PLAYER_SHOOT_PARAMETER   = "mode";

constexpr std::string_view CHANGE_MODE_EVENT		= "event:/player/change_mode"; //prueba
//constexpr std::string_view CHANGE_MODE_PARAMETER    = "mode"


constexpr std::string_view DASH_PLAYER_EVENT        = "event:/player/dash_player";

constexpr std::string_view DAMAGE_PLAYER_EVENT      = "event:/player/damage_player";

//MUSIC
constexpr std::string_view BACKGROUND_MUSIC_EVENT 	= "event:/music/main_music";
constexpr std::string_view BACKGROUND_MUSIC_EVENT_4 = "event:/music/main_music_4";

//ENEMY
constexpr std::string_view ASSEMBLED_ATTACK_EVENT   = "event:/enemies/assembled_attack";

//OBJECTS
constexpr std::string_view PICKUP_KEY_EVENT         = "event:/objects/pickup_key";
constexpr std::string_view OPEN_DOOR_EVENT          = "event:/objects/open_door";
constexpr std::string_view HITMARKER_EVENT          = "event:/objects/hitmarker";

//AMBIENT

