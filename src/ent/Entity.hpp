#pragma once

#include <util/Alias.hpp>

struct Component;

struct Transformable;
struct Velocity;
struct Physics;
struct BulletData;
struct CharacterData;
struct BoundingBox;
struct AI;

struct Entity
{
	explicit Entity(EntityType type);

	~Entity() = default;

	explicit operator bool() const { return type != UNDEFINED; }

	inline static void resetIDManagementValue() { nextID = 0; }

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

		inline static EntityID nextID { 0 };

		EntityType type { UNDEFINED };
		EntityID 	ID  {     0	  	};
};

// TODO: generalizar entidades

