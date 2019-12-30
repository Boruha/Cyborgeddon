#pragma once

#include <cmp/Transformable.hpp>
#include <cmp/Velocity.hpp>
#include <cmp/BoundingBox.hpp>
#include <cmp/AI.hpp>

#include <ent/Entity.hpp>

#include <SunlightEngine/Vector3.hpp>

using Sun::Vector3f;

struct EntityEnemy : Entity
{
	explicit EntityEnemy() : Entity(ENEMY) {  }

	explicit EntityEnemy(Physics& physics, Velocity& velocity, BoundingBox& box, AI& aiComponent, SceneNode& node) :
			Entity(ENEMY), physics(&physics), velocity(&velocity), collider(&box), ai(&aiComponent), node(&node)
	{

	}

	~EntityEnemy() override {
		std::cout << "Muere un enemigo" << std::endl;
		node->removeFromScene();
	};

	Physics*			  physics { nullptr };
	Velocity*			 velocity { nullptr };
	BoundingBox*		 collider { nullptr };
	AI* 					   ai { nullptr };
	SceneNode*				 node { nullptr };
};

// TODO: Crear componente de IA con su enum de estados