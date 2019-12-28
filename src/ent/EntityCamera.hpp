#pragma once

#include <cmp/Transformable.hpp>
#include <cmp/Velocity.hpp>

#include <ent/Entity.hpp>

#include <SunlightEngine/Device.hpp>
#include <SunlightEngine/GenericNode.hpp>
#include <SunlightEngine/Vector3.hpp>
#include <SunlightEngine/CameraNode.hpp>

using Sun::Device;
using Sun::Vector3f;
using Sun::CameraNode;

struct EntityCamera : Entity
{
	explicit EntityCamera(Transformable& transformable, const Velocity& velocity, CameraNode& node)
		: Entity(CAMERA_ID), transformable(&transformable), velocity(&velocity), node(&node)
	{

	}

	Transformable* 		transformable { nullptr };
	const Velocity*			 velocity { nullptr };
	CameraNode* 				 node { nullptr };
};