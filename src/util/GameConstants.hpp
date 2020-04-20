#pragma once

#include <Engine/util/Color.hpp>

constexpr std::string_view GAME_NAME	= "Cyborgeddon";

constexpr unsigned  UPDATES_PER_SECOND  = 60;
constexpr double    FIXED_DELTA_TIME    = 1.0 / UPDATES_PER_SECOND;

constexpr unsigned  VIEWPORT_WIDTH      = 1280;
constexpr unsigned  VIEWPORT_HEIGHT     = 720;

constexpr Color     BACKGROUND_COLOR    = Color(0x330044FF);
