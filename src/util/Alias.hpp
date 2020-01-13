#pragma once

#include <memory>
#include <cstdint>
#include <SunlightEngine/GenericNode.hpp>

enum EntityType : uint8_t { UNDEFINED, PLAYER, CAMERA, ENEMY, KEY, DOOR, BULLET, FLOOR, WALL };

enum AI_State : uint8_t { PATROL_STATE, PURSUE_STATE, ATTACK_STATE, END_STATE }; // END_STATE debe estar S I E M P R E al final

enum ColliderType : uint8_t { DYNAMIC, STATIC, RAY, END_TYPE }; // END_TYPE debe estar S I E M P R E al final

using Node_ptr = std::unique_ptr<Sun::GenericNode>;