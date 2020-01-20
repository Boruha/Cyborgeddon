#pragma once

#include <cstdint>

enum ComponentType : uint16_t 	{
									AI_TYPE,
									NODE_TYPE,
									VELOCITY_TYPE,
									TRANSFORMABLE_TYPE,
									PHYSICS_TYPE,
									CHARACTER_DATA_TYPE,
									BULLET_DATA_TYPE,
									SPECIAL_BOUNDING_BOX_TYPE,
									STATIC_BOUNDING_BOX_TYPE,
									RAY_BOUNDING_BOX_TYPE
								};
