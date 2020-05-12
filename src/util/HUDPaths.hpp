#pragma once

#include <string_view>

// INGAME

constexpr std::string_view KEY_1 = "resources/hud/llave_1.png";
constexpr std::string_view KEY_2 = "resources/hud/llave_2.png";

constexpr std::string_view MAIN_HUD = "resources/hud/marco_negro.png";

constexpr std::string_view ANGEL_LIFEBAR = "resources/hud/barra_vida_azul.png";
constexpr std::string_view DEMON_LIFEBAR = "resources/hud/barra_vida_roja.png";

constexpr std::string_view BACKGROUND_LIFEBAR = "resources/hud/fondo_blanco.png";

constexpr unsigned int NUM_INGAME_TEXTURES = 6;

// PAUSE

constexpr std::string_view PAUSE_TEXTURE = "resources/menu/pause/fondo.png";

constexpr std::string_view PAUSE_CONTINUE = "resources/menu/pause/pausa_continue.png";
constexpr std::string_view PAUSE_OPTIONS = "resources/menu/pause/pausa_options.png";
constexpr std::string_view PAUSE_EXIT = "resources/menu/pause/pausa_exit.png";

constexpr unsigned int NUM_PAUSE_OPTIONS = 3;
