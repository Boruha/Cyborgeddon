#pragma once

#include <cmp/AI.hpp>
#include <ent/Entity.hpp>
#include <SunlightEngine/Device.hpp>

struct EntityEnemy : Entity
{
	explicit EntityEnemy(Transformable& transformable, Velocity& velocity, const Vector3f& dim, AI& aiComponent, SceneNode& node) :
		Entity(ENEMY_ID), transformable(&transformable), velocity(&velocity), collider(dim, transformable.position), ai(&aiComponent), node(&node)
	{

	}

	~EntityEnemy() {
		std::cout << "Muere un enemigo" << std::endl;
		node->removeFromScene();
	};

	Transformable*	transformable { nullptr };
	Velocity*			 velocity { nullptr };
	BoundingBox	collider;
	AI* 					   ai { nullptr };
	SceneNode*				 node { nullptr };

	// 0 stopped, 1 chasing, -1 dead.
	Alive 						alive;
};

// TODO: Crear componente de IA con su enum de estados