#include <sys/DeathSystem.hpp>
#include <Engine/util/Math.hpp>

void DeathSystem::init(){
    deathMessages.reserve(16);
}

void DeathSystem::update(const std::unique_ptr<GameContext> &context, const float deltaTime) {

    for(const auto & message : deathMessages)
        context->addToDestroy(message.ID);

    deathMessages.clear();
}

