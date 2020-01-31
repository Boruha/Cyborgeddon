#pragma once

// PLAYER
constexpr float     PLAYER_SPEED					= 30.f;
constexpr float     PLAYER_DASH_SPEED               = PLAYER_SPEED * 20.f;
constexpr float     PLAYER_ACCELERATION				= PLAYER_DASH_SPEED * 10.f;
constexpr float     PLAYER_HEALTH					= 100.f;
constexpr float     PLAYER_ATTACK_DAMAGE			= 10.f;
constexpr float     PLAYER_ATTACKING_COOLDOWN		= 1.f / 8.f;
constexpr float     PLAYER_SWITCH_MODE_COOLDOWN		= 5.f;
constexpr float     PLAYER_DASH_COOLDOWN            = 5.f;
constexpr float     PLAYER_ROTATION_SPEED			= 270.f;

// ENEMY
constexpr unsigned  MAX_PATROL_POSITIONS 	        = 6u;

constexpr float     ENEMY_SPEED						= 40.f;
constexpr float     ENEMY_DASH_SPEED                = ENEMY_SPEED * 0.f;
constexpr float     ENEMY_ACCELERATION				= ENEMY_DASH_SPEED * 0.f;
constexpr float     ENEMY_HEALTH					= 100.f;
constexpr float     ENEMY_ATTACK_DAMAGE				= 20.f;
constexpr float     ENEMY_ATTACKING_COOLDOWN		= 1.f / 2.f;
constexpr float     ENEMY_SWITCH_MODE_COOLDOWN		= 0.f;
constexpr float     ENEMY_DASH_COOLDOWN             = 5.f;
constexpr float     ENEMY_ROTATION_SPEED			= 270.f;

// ATTACK
constexpr float     BULLET_SPEED					= 400.f;
constexpr float     FACTOR_DIFFERENT_MODE			= 2.f;
constexpr float     FACTOR_SAME_MODE				= 1.f / FACTOR_DIFFERENT_MODE;
