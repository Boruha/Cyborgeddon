#pragma once

#include <cmp/Components.hpp>

#include <SunlightEngine/Device.hpp>
#include <util/Alias.hpp>

#include <unordered_map>
#include <util/ComponentVectorType.hpp>

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

	[[nodiscard]] const variantComponentVectorTypes& getComponents(const ComponentType type)  const { return const_cast<std::unordered_map<ComponentType , variantComponentVectorTypes> &>(map)[type]; }
	[[nodiscard]] 		variantComponentVectorTypes& getComponents(const ComponentType type) 		{ return const_cast<variantComponentVectorTypes&>(std::as_const(*this).getComponents(type)); }

private:

	std::unordered_map <ComponentType, variantComponentVectorTypes> map;
};