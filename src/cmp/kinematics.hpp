#pragma once

#include <irrlicht/vector3d.h>

struct Kinematic
{
/*	Vector3f	velocity;	*/
    //normaliced vector which indicates the direction of our movement.
	irr::core::vector3df vDirection = irr::core::vector3df(0,0,0);
    const float maxVel { 5.f };
};