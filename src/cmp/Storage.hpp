#pragma once

#include <vector>

#include <cmp/Components.hpp>
#include <SunlightEngine/GenericNode.hpp>
#include <SunlightEngine/Device.hpp>
#include <SunlightEngine/SceneNode.hpp>
#include <SunlightEngine/CameraNode.hpp>

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

	Lock& 	createLock(const EntityType& e_type, const std::size_t& e_ID);
	AI&		createAI(const EntityType& e_type, const std::size_t& e_ID, const vector<Vector3f>& patrol);

	SceneNode& createSceneNode  (const Device& device, const Vector3f& position, const Vector3f& rotation, const Vector3f& dim, const char* mesh = nullptr, const char* texture = nullptr);
	CameraNode& createCameraNode(const Device& device, const Vector3f& position, const Vector3f& target);

	Transformable& createTransformable(const EntityType& e_type, const std::size_t& e_ID, const Vector3f& pos = Vector3f(), const Vector3f& rot = Vector3f());

	Velocity& createVelocity(const EntityType& e_type, const std::size_t& e_ID, const float& speed);

	BoundingBox& createBoundingBox(const EntityType& e_type, const std::size_t& e_ID, const Vector3f& dim, const Vector3f& pos, bool is_static = false);

	Physics& createPhysics(const EntityType& e_type, const std::size_t& e_ID, const Vector3f& pos = Vector3f(), const Vector3f& vel = Vector3f(), const Vector3f& rot = Vector3f());

	[[nodiscard]] const vector<Lock>& getLockComponents() const { return lockComponents; }
	[[nodiscard]] 		vector<Lock>& getLockComponents() 	   	{ return lockComponents; }

	[[nodiscard]] const vector<AI>&   getAIComponents()   const { return   AIComponents; }
	[[nodiscard]] 		vector<AI>&   getAIComponents() 	   	{ return   AIComponents; }

	[[nodiscard]] const vector<SceneNode>&   getSceneNodeComponents() const { return sceneNodeComponents; }
	[[nodiscard]] 		vector<SceneNode>&   getSceneNodeComponents() 	 	{ return sceneNodeComponents; }

	[[nodiscard]] const vector<CameraNode>&   getCameraNodeComponents() const { return cameraNodeComponents; }
	[[nodiscard]] 		vector<CameraNode>&   getCameraNodeComponents()		  { return cameraNodeComponents; }

	[[nodiscard]] const vector<Transformable>&   getTransformableComponents() const { return   transformableComponents; }
	[[nodiscard]] 		vector<Transformable>&   getTransformableComponents() 	   	{ return   transformableComponents; }

	[[nodiscard]] const vector<Velocity>&   getVelocityComponents() const { return   velocityComponents; }
	[[nodiscard]] 		vector<Velocity>&   getVelocityComponents()	   	  { return   velocityComponents; }

	[[nodiscard]] const vector<BoundingBox>&   getBoundingComponents() const { return   boundingComponents; }
	[[nodiscard]] 		vector<BoundingBox>&   getBoundingComponents()	   	 { return   boundingComponents; }

	[[nodiscard]] const vector<Physics>&   getPhysicsComponents() const { return   physicsComponents; }
	[[nodiscard]] 		vector<Physics>&   getPhysicsComponents()	   	{ return   physicsComponents; }

private:
	vector<Lock> 			   	    	 lockComponents;
	vector<AI> 		  			     	   AIComponents;
	vector<SceneNode> 	  		    sceneNodeComponents;
	vector<CameraNode>	 		   cameraNodeComponents;
	vector<Transformable> 		transformableComponents;
	vector<Velocity>				 velocityComponents;
	vector<BoundingBox>		  		 boundingComponents;
	vector<Physics>					  physicsComponents;
};