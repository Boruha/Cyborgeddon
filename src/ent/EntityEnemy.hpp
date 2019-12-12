#pragma once

#include <ent/Entity.hpp>
#include <SunlightEngine/Device.hpp>

struct EntityEnemy : Entity
{
	explicit EntityEnemy(const Device& device, const Vector3f& pos = Vector3f(), const Vector3f& dim = Vector3f(10), const float& speed = 0.1f)
	: Entity(ENEMY_ID), transformable(pos), collider(dim), velocity(speed), node(device, pos, dim) { node.setTexture(renderable.texture); }

	Renderable					renderable {"", "./img/textures/testing/testing_enemy.png"};
	Transformable 				transformable;
	BoundingBox					collider;
	Velocity 					velocity;

	SceneNode node;

	//0 stopped, 1 chasing.
	int 	ai_state { 0 }; 
};

// TODO: Crear componente de IA con su enum de estados