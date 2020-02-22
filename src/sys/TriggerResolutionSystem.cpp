#include <sys/TriggerResolutionSystem.hpp>
#include <util/SoundPaths.hpp>

void TriggerResolutionSystem::update(const std::unique_ptr<GameContext> &context, float deltaTime) {
    for (const auto & message : triggerMessages) {
        if (message.type2 == PLAYER) {
            if (message.type1 == KEY) {
                // TODO : abrir puerta (con mensajes? o context?)
                soundMessages.emplace_back(PICKUP_KEY_EVENT);
                deathMessages.emplace_back(message.ID1);
            } else if (message.type1 == DOOR) {
                soundMessages.emplace_back(OPEN_DOOR_EVENT);
                deathMessages.emplace_back(message.ID1);
            }
        }
    }

    triggerMessages.clear();
}