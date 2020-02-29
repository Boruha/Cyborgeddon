#include <cmp/ComponentPool.hpp>

void ComponentPool::initData(const unsigned size) {
/*
	initVector<AI>(5);
	initVector<Transformable>(59);
	initVector<Velocity>(8);
	initVector<Physics>(9);
	initVector<BulletData>(2);
	initVector<CharacterData>(8);
	initVector<TriggerMovSphere>(6);
	initVector<TriggerStaticAABB>(16);
	initVector<RigidStaticAABB>(49);
	initVector<TriggerFastMov>(2);
	initVector<Graph>(1);
	initVector<Render>(68);
*/
	initVector<AI>(size);
	initVector<Transformable>(size);
	initVector<Velocity>(size);
	initVector<Physics>(size);
	initVector<BulletData>(size);
	initVector<CharacterData>(size);
	initVector<TriggerMovSphere>(size);
	initVector<TriggerStaticAABB>(size);
	initVector<RigidStaticAABB>(size);
	initVector<TriggerFastMov>(size);
	initVector<Graph>(size);
	initVector<Render>(size);
}

void ComponentPool::cleanData() {
	Component::resetIDManagementValue();	// ID de los componentes a 0

	map.clear();
}
