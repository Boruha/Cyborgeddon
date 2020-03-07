#include <cmp/Storage.hpp>

void Storage::initData(const unsigned maxComponents) {
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
	initVector<AI>(maxComponents);
	initVector<Transformable>(maxComponents);
	initVector<Velocity>(maxComponents);
	initVector<Physics>(maxComponents);
	initVector<BulletData>(maxComponents);
	initVector<CharacterData>(maxComponents);
	initVector<TriggerMovSphere>(maxComponents);
	initVector<TriggerStaticAABB>(maxComponents);
	initVector<RigidStaticAABB>(maxComponents);
	initVector<TriggerFastMov>(maxComponents);
	initVector<Graph>(maxComponents);
	initVector<Render>(maxComponents);
}

void Storage::cleanData() {
	Component::resetIDManagementValue();	// ID de los componentes a 0

	map.clear();
}