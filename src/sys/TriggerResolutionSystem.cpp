#include <sys/TriggerResolutionSystem.hpp>
#include <util/SoundPaths.hpp>
#include <Engine/util/Math.hpp>

// TODO : refactorizar codigo !!!!!!

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
        } else if (message.type1 == BULLET) {
        	if (message.type2 == ENEMY) {
        		auto& bullet = *context->getEntityByID(message.ID1).bulletData;
        		auto& character = *context->getEntityByID(message.ID2).characterData;

		        if (character.mode == NEUTRAL) {
			        character.health -= bullet.damage;
		        } else {
			        if (character.mode != bullet.damageType)
				        character.health -= (bullet.damage * FACTOR_DIFFERENT_MODE);
			        else
				        character.health -= (bullet.damage * FACTOR_SAME_MODE);
		        }

		        soundMessages.emplace_back(HITMARKER_EVENT);

		        if (!greater_e(character.health, 0))
			        deathMessages.emplace_back(character.getEntityID());
        	}
        }
    }

    triggerMessages.clear();
}