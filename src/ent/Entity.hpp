#pragma once

#include <util/Alias.hpp>
#include <Engine/EngineInterface/SceneInterface/INode.hpp>

struct Transformable;
struct Velocity;
struct Physics;
struct BulletData;
struct CharacterData;
struct AI;
struct TriggerMovSphere;
struct TriggerStaticAABB;
struct RigidStaticAABB;
struct TriggerFastMov;

struct Entity
{
	explicit Entity(EntityType type);

	~Entity() = default;

	explicit operator bool() const { return type != UNDEFINED; }

	inline static void resetIDManagementValue() { nextID = 0; }

	void makeUndefined();

	[[nodiscard]] const EntityType& getType() 	const { return  type; }
	[[nodiscard]] const EntityID&  	getID() 	const { return 	  ID; }

	Transformable		* transformable 	{ nullptr };
	Velocity		  	* velocity 			{ nullptr };
	Physics			  	* physics			{ nullptr };
	BulletData		  	* bulletData		{ nullptr };
	CharacterData	  	* characterData		{ nullptr };
	AI				  	* ai				{ nullptr };
	TriggerStaticAABB 	* triggStaticAABB 	{ nullptr };
	TriggerMovSphere  	* triggerMovSphere	{ nullptr };
	TriggerFastMov      * triggerFastMov    { nullptr };
	RigidStaticAABB   	* rigidStaticAABB	{ nullptr };

	INode				* inode 			{ nullptr };

	private:

		inline static EntityID nextID { 0 };

		EntityType type { UNDEFINED };
		EntityID 	ID  {     0	  	};
};

// TODO: generalizar entidades

