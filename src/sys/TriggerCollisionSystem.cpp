#include <sys/TriggerCollisionSystem.hpp>
#include <Engine/util/MathIntersection.hpp>

void TriggerCollisionSystem::init() {
    triggerMessages.reserve(16);
}

void TriggerCollisionSystem::update(const std::unique_ptr<GameContext> &context, float deltaTime) {
    for (const auto & trigger : context->getComponents().get<TriggerStaticAABB>())
        for(const auto & boundingbox : context->getComponents().get<BoundingBox>())
            if(intersectionAABB(trigger.min, trigger.max, boundingbox.min, boundingbox.max))
                triggerMessages.emplace_back(trigger.getEntityType(), trigger.getEntityID(), boundingbox.getEntityType(), boundingbox.getEntityID());
}