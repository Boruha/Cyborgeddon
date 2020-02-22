#include <cmp/ComponentPool.hpp>

void ComponentPool::initData(const unsigned size) {
	initVector<AI>(size);
	initVector<Transformable>(size);
	initVector<Velocity>(size);
	initVector<BoundingBox>(size);	// TODO : remplazar este bounding por rigids y triggers
	initVector<Physics>(size);
	initVector<BulletData>(size);
	initVector<CharacterData>(size);
	initVector<RigidMovSphere>(size);
	initVector<TriggerStaticAABB>(size);
}

void ComponentPool::cleanData() {
	Component::resetIDManagementValue();	// ID de los componentes a 0

	map.clear();
}
