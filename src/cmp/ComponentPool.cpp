#include <cmp/ComponentPool.hpp>

void ComponentPool::initData(const unsigned size) {
	initVector<AI>(8);
	initVector<Transformable>(48);
	initVector<Velocity>(8);
	initVector<Physics>(16);
	initVector<BulletData>(4);
	initVector<CharacterData>(8);
	initVector<TriggerMovSphere>(8);
	initVector<TriggerStaticAABB>(16);
	initVector<RigidStaticAABB>(48);
	initVector<TriggerFastMov>(4);
	initVector<Graph>(1);
}

void ComponentPool::cleanData() {
	Component::resetIDManagementValue();	// ID de los componentes a 0

	map.clear();
}
