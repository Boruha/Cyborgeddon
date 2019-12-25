#pragma once

#include <ent/Entity.hpp>
#include <SunlightEngine/Device.hpp>

struct EntityEnemy : Entity
{
	explicit EntityEnemy(const Device& device, const Vector3f& pos, const Vector3f& dim, const float& speed = 0.1f) :
		Entity(ENEMY_ID), transformable(pos), collider(dim, pos), velocity(speed), node(device, pos, dim)
	{
		node.setTexture(renderable.texture);
	}

	Renderable					renderable {"", "./img/textures/testing/testing_enemy.png"};
	Transformable 				transformable;
	BoundingBox					collider;
	Velocity 					velocity;

	SceneNode 					node;

	//0 stopped, 1 chasing, -1 dead.
	int 	ai_state { 0 };
	bool 	alive = { true };
};

// TODO: Crear componente de IA con su enum de estados