#include <cmp/ComponentPool.hpp>

void ComponentPool::initData(const unsigned size) {
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
	initVector<Render>(size);
}

void ComponentPool::cleanData() {
	Component::resetIDManagementValue();	// ID de los componentes a 0

	map.clear();
}
