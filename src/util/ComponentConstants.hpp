#pragma once

// CMP AI
constexpr unsigned int MAX_PATROL_POSITIONS 	= 6;

// CMP PHYSICS
constexpr float PLAYER_ROTATION_SPEED			= 270.f;

// CMP VELOCITY
constexpr float PLAYER_SPEED					= 30.f;
constexpr float PLAYER_DASH_SPEED 				= PLAYER_SPEED * 20.f;
constexpr float PLAYER_ACCELERATION				= PLAYER_DASH_SPEED * 10.f;

constexpr float ENEMY_SPEED						= 40.f;
constexpr float ENEMY_ACCELERATION				= 0.f;

constexpr float BULLET_SPEED					= 400.f;

// CMP CHARACTERDATA
constexpr float PLAYER_HEALTH					= 100.f;
constexpr float PLAYER_ATTACK_DAMAGE			= 50.f;
constexpr float PLAYER_ATTACKING_COOLDOWN		= 1.f / 8.f;

constexpr float ENEMY_HEALTH					= 100.f;
constexpr float ENEMY_ATTACK_DAMAGE				= 20.f;
constexpr float ENEMY_ATTACKING_COOLDOWN		= 1.f / 2.f;

constexpr float FACTOR_DIFFERENT_MODE			= 2.f;
constexpr float FACTOR_SAME_MODE				= 1.f / FACTOR_DIFFERENT_MODE;
