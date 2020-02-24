#include <sys/TriggerCollisionSystem.hpp>
#include <Engine/util/MathIntersection.hpp>

void TriggerCollisionSystem::init() {
    triggerMessages.reserve(16);
}

void TriggerCollisionSystem::update(const Context &context, const float deltaTime) {
    for (const auto & trigger : context->getComponents().get<TriggerStaticAABB>())
    	if (trigger)
	        for(const auto & boundingbox : context->getComponents().get<BoundingBox>())
	        	if (boundingbox)
	                if(intersectionAABB(trigger.min, trigger.max, boundingbox.min, boundingbox.max))
                        triggerMessages.emplace_back(trigger.getEntityType(), trigger.getEntityID(), boundingbox.getEntityType(), boundingbox.getEntityID());
}

void TriggerCollisionSystem::reset() {
	triggerMessages.clear();
	triggerMessages.reserve(16);
}
