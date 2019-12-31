#pragma once

enum EntityType : std::size_t { UNDEFINED, PLAYER, CAMERA, ENEMY, KEY, DOOR, BULLET, FLOOR, WALL };

enum AI_State : std::size_t { PATROL_STATE, PURSUE_STATE, ATTACK_STATE, END_STATE }; // END_STATE debe estar S I E M P R E al final

enum ColliderType : std::size_t { DYNAMIC, STATIC, RAY, END_TYPE }; // END_TYPE debe estar S I E M P R E al final