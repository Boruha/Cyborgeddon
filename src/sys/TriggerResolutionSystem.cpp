#include <sys/TriggerResolutionSystem.hpp>
#include <util/SoundPaths.hpp>

void TriggerResolutionSystem::update(const Context &context, float deltaTime) {
    for (const auto & message : triggerMessages) {
        if (message.type1 == PLAYER) {
            if (message.type2 == KEY) {
                // TODO : abrir puerta (con mensajes? o context?)
                context->getEntityByID(message.ID2 - 1).rigidStaticAABB->makeUndefined();
                soundMessages.emplace_back(PICKUP_KEY_EVENT);
                deathMessages.emplace_back(message.ID2);
            } else if (message.type2 == DOOR) {
                soundMessages.emplace_back(OPEN_DOOR_EVENT);
                deathMessages.emplace_back(message.ID2);
            }
        }
    }

    triggerMessages.clear();
}