#pragma once

#include <ent/Entity.hpp>
#include <SunlightEngine/Device.hpp>

struct EntityEnemy : Entity
{
	EntityEnemy() = default;
	explicit EntityEnemy(const Device& device, const Vector3f& pos = Vector3f(0,0,0), const Vector3f& dim = Vector3f(10), const float& speed = 0.1f)
	: Entity(ENEMY_ID), transformable(pos), collider(dim, transformable), velocity(speed), node(device, pos, dim) { node.setTexture(renderable.texture); }
	~EntityEnemy() override = default;

	Renderable					renderable {"", "./img/textures/testing/testing_cube.png"};
	Transformable 				transformable;
	BoundingBox					collider;
	Velocity 					velocity;

	SceneNode node;

	//0 stopped, 1 chasing.
	int 	ai_state { 0 }; 
};