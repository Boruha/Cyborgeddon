#pragma once

// SOUND BANKS
constexpr std::string_view MASTER_BANK				= "./resources/sounds/banks/Master.bank";
constexpr std::string_view MASTER_STRINGS_BANK 		= "./resources/sounds/banks/Master.strings.bank";

// SOUND EVENTS

//PLAYER
constexpr std::string_view PLAYER_SHOOT_EVENT       = "event:/player/disparo_player";
constexpr std::string_view CHANGE_MODE_EVENT		= "event:/player/change_mode";
constexpr std::string_view DASH_PLAYER_EVENT        = "event:/player/dash_player";

//MUSIC
constexpr std::string_view BACKGROUND_MUSIC_EVENT 	= "event:/music/main_music";

//ENEMY
constexpr std::string_view ASSEMBLED_ATTACK_EVENT   = "event:/enemies/assembled_attack";

//OBJECTS
constexpr std::string_view KEY_DOOR_EVENT           = "event:/objects/action_key_door";
constexpr std::string_view DAMAGE_EVENT             = "event:/objects/damage";

//AMBIENT

