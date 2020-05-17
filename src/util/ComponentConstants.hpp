#pragma once

/*TODO: CAMBIOS A INTRODUCIR HABLADOS CON MATEO:
 *
 * El dash tendra un cooldown de 2~3 segundos.
 * A cambio, el personaje podrá cambiar de modo cada 1 segundo.
 * Cada vez que cambiemos de modo, el dash volvera a estar disponible.
 *
 * AHORA MISMO EL DASH Y EL CAMBIO DE MODO ESTAN PUESTOS A 1 SEGUNDO PARA QUE PODAMOS PROBAR COSAS MAS FACILMENTE.
 */

// PLAYER
constexpr float     PLAYER_SPEED					= 10.f;
constexpr float     PLAYER_DASH_SPEED               = PLAYER_SPEED * 20.f;
constexpr float     PLAYER_ACCELERATION				= PLAYER_DASH_SPEED * 10.f;
constexpr float     PLAYER_HEALTH					= 100.f;
constexpr float     PLAYER_ATTACK_DAMAGE			= 10.f;
constexpr float     PLAYER_ATTACKING_COOLDOWN		= 1.f / 8.f;
constexpr float     PLAYER_SWITCH_MODE_COOLDOWN		= 1.f;
constexpr float     PLAYER_DASH_COOLDOWN            = 1.f;
constexpr float     PLAYER_ROTATION_SPEED			= 270.f;

// ENEMY
constexpr unsigned  MAX_PATROL_POSITIONS 	        = 6u;

constexpr float     ENEMY_SPEED						= 10.f;
constexpr float     ENEMY_DASH_SPEED                = ENEMY_SPEED * 0.f;
constexpr float     ENEMY_ACCELERATION				= ENEMY_DASH_SPEED * 0.f;
constexpr float     ENEMY_HEALTH					= 100.f;
constexpr float     ENEMY_ATTACK_DAMAGE				= 20.f;
constexpr float     ENEMY_ATTACKING_COOLDOWN		= 1.f / 2.f;
constexpr float     ENEMY_SWITCH_MODE_COOLDOWN		= 0.f;
constexpr float     ENEMY_DASH_COOLDOWN             = 5.f;
constexpr float     ENEMY_ROTATION_SPEED			= 270.f;

// ATTACK PLAYER
constexpr float     BULLET_SPEED					= 600.f;
constexpr float     FACTOR_DIFFERENT_MODE			= 2.f;
constexpr float     FACTOR_SAME_MODE				= 1.f / FACTOR_DIFFERENT_MODE;

// ATTACK ENEMY
constexpr float     MELEE_ATTACK_RANGE   		    = 2.5f;
constexpr float     DIST_ATTACK_RANGE   		    = 20.f;
constexpr float     DIST_ATTACK_RANGE2  		    = DIST_ATTACK_RANGE  * DIST_ATTACK_RANGE;
constexpr float     MELEE_ATTACK_RANGE2  		    = MELEE_ATTACK_RANGE * MELEE_ATTACK_RANGE;

//DEMON JUMP
constexpr float     JUMP_COOLDOWN                   = 8.f;
constexpr float     JUMP_TIMER                      = 1.f;
constexpr float     JUMP_AREA_DMG                   = 20.f;
constexpr float     JUMP_AREA_DMG2                  = JUMP_AREA_DMG * JUMP_AREA_DMG; 

//NAV GRAPH
constexpr unsigned  MAX_GRAPH_CONN                  = 3;

//AI_STATES FREQUENCY_SCHEDULE
constexpr unsigned  LOD_PA_STATE                    = 11;
constexpr unsigned  PATROL_STATE                    = 7;
constexpr unsigned  PURSUE_STATE                    = 3;
constexpr unsigned  ATTACK_STATE                    = 2;

