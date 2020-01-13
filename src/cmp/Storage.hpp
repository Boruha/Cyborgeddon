#pragma once

#include <vector>

#include <cmp/Components.hpp>

#include <SunlightEngine/Device.hpp>
#include <SunlightEngine/SceneNode.hpp>
#include <SunlightEngine/CameraNode.hpp>

#include <util/Alias.hpp>

using std::vector;
using Sun::SceneNode;
using Sun::CameraNode;
using Sun::Device;

struct Storage {
	explicit Storage(std::size_t);
	~Storage();

	Storage(			   ) = delete;
	Storage(const Storage& ) = delete;
	Storage(	  Storage&&) = delete;

	Storage& operator=(const Storage& ) = delete;
	Storage& operator=(		 Storage&&) = delete;

	AI&		createAI(EntityType e_type, std::size_t e_ID, const vector<Vector3f>& patrol);

	Node_ptr& createSceneNode  (const Device& device, const Vector3f& position, const Vector3f& rotation, const Vector3f& dim, const char* mesh = nullptr, const char* texture = nullptr);
	Node_ptr& createCameraNode(const Device& device, const Vector3f& position, const Vector3f& target);

	Transformable& createTransformable(EntityType e_type, std::size_t e_ID, const Vector3f& pos = Vector3f(), const Vector3f& rot = Vector3f());

	Velocity& createVelocity(EntityType e_type, std::size_t e_ID, float speed, float acceleration);
	Velocity& createVelocity(EntityType e_type, std::size_t e_ID, const Vector3f& dir, float speed, float acceleration);

	BoundingBox& createBoundingBox(EntityType e_type, std::size_t e_ID, const Vector3f& dim, const Vector3f& pos, Vector3f& vel, bool passable, ColliderType type, bool canMutate);

	Physics& createPhysics(EntityType e_type, std::size_t e_ID, const Vector3f& pos = Vector3f(), const Vector3f& vel = Vector3f(), const Vector3f& rot = Vector3f());

	BulletData& createBulletData(EntityType e_type, std::size_t e_ID, float speed, bool type, float damage);

	CharacterData& createCharacterData(EntityType e_type, std::size_t e_ID, bool mode, float health, float attackDamage, float attackingCooldown);

	[[nodiscard]] const vector<AI>&   getAIComponents()   const { return   AIComponents; }
	[[nodiscard]] 		vector<AI>&   getAIComponents() 	   	{ return   AIComponents; }

	[[nodiscard]] const vector<Node_ptr>&   getNodeComponents() const { return nodeComponents; }
	[[nodiscard]] 		vector<Node_ptr>&   getNodeComponents() 	  { return nodeComponents; }

	[[nodiscard]] const vector<Transformable>&   getTransformableComponents() const { return   transformableComponents; }
	[[nodiscard]] 		vector<Transformable>&   getTransformableComponents() 	   	{ return   transformableComponents; }

	[[nodiscard]] const vector<Velocity>&   getVelocityComponents() const { return   velocityComponents; }
	[[nodiscard]] 		vector<Velocity>&   getVelocityComponents()	   	  { return   velocityComponents; }

	[[nodiscard]] const vector<BoundingBox>&   getStaticBoundingComponents() const { return   staticBoundingComponents; }
	[[nodiscard]] 		vector<BoundingBox>&   getStaticBoundingComponents()	   { return   staticBoundingComponents; }

	[[nodiscard]] const vector<BoundingBox>&   getBoundingComponents() const { return   boundingComponents; }
	[[nodiscard]] 		vector<BoundingBox>&   getBoundingComponents()	   	 { return   boundingComponents; }

	[[nodiscard]] const vector<BoundingBox>&   getRayBoundingComponents() const { return   rayBoundingComponents; }
	[[nodiscard]] 		vector<BoundingBox>&   getRayBoundingComponents()	   	{ return   rayBoundingComponents; }

	[[nodiscard]] const vector<Physics>&   getPhysicsComponents() const { return   physicsComponents; }
	[[nodiscard]] 		vector<Physics>&   getPhysicsComponents()	   	{ return   physicsComponents; }

	[[nodiscard]] const vector<BulletData>&   getBulletDataComponents() const { return   bulletDataComponents; }
	[[nodiscard]] 		vector<BulletData>&   getBulletDataComponents()	   	  { return   bulletDataComponents; }

	[[nodiscard]] const vector<CharacterData>&   getCharacterDataComponents() const { return   characterDataComponents; }
	[[nodiscard]] 		vector<CharacterData>&   getCharacterDataComponents()	   	{ return   characterDataComponents; }

private:
	vector<AI> 		  		   		   AIComponents;
	vector<Node_ptr>		 		 nodeComponents;
	vector<Transformable> 	transformableComponents;
	vector<Velocity>			 velocityComponents;
	vector<BoundingBox>		 	 boundingComponents;
	vector<BoundingBox>	   staticBoundingComponents;
	vector<BoundingBox>		  rayBoundingComponents;
	vector<Physics>				  physicsComponents;
	vector<BulletData>		   bulletDataComponents;
	vector<CharacterData>	characterDataComponents;
};