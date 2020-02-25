#include <sys/TriggerCollisionSystem.hpp>
#include <Engine/util/MathIntersection.hpp>

void TriggerCollisionSystem::init() {
    triggerMessages.reserve(16);
}

void TriggerCollisionSystem::update(const Context &context, const float deltaTime) {
	for (auto & sphere : context->getComponents().get<TriggerMovSphere>()) {
		if (sphere) {
			for (const auto & aabb : context->getComponents().get<TriggerStaticAABB>())
				if (aabb && intersectionSphereAABB(sphere.position, sphere.radius, aabb.min, aabb.max))
					triggerMessages.emplace_back(sphere.getEntityType(), sphere.getEntityID(), aabb.getEntityType(), aabb.getEntityID());
		}
	}
}

void TriggerCollisionSystem::reset() {
	triggerMessages.clear();
	triggerMessages.reserve(16);
}
