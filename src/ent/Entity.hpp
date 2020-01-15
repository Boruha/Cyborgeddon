#pragma once

#include <util/Alias.hpp>

#include <cmp/Components.hpp>

struct Entity
{
	explicit Entity(EntityType type);

	~Entity() = default;

	void addComponent(Component&);
	void makeUndefined();

	[[nodiscard]] const EntityType& getType() 	const { return  type; }
	[[nodiscard]] const EntityID&  	getID() 	const { return 	  ID; }

	Transformable* 	transformable 	{ nullptr };
	Velocity* 		velocity 		{ nullptr };
	Physics*		physics			{ nullptr };
	BulletData*		bulletData		{ nullptr };
	CharacterData*	characterData	{ nullptr };
	BoundingBox*	collider		{ nullptr };
	AI*				ai				{ nullptr };

	Node_ptr* 		node			{ nullptr };

	private:
		std::vector<Component*> components;

		inline static EntityID nextID { 0 };

		EntityType type { UNDEFINED };
		EntityID 	ID  {     0	  	};
};

// TODO: generalizar entidades

