#include <sys/DeathSystem.hpp>

void DeathSystem::init(){
    deathMessages.reserve(16);
}

void DeathSystem::update(const Context &context, const float deltaTime) {

    for(const auto & message : deathMessages)
        context->addToDestroy(message.ID);

    deathMessages.clear();
}

void DeathSystem::reset() {
	deathMessages.clear();
	deathMessages.reserve(16);
}

