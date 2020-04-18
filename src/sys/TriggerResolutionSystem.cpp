#include <sys/TriggerResolutionSystem.hpp>
#include <util/SoundPaths.hpp>
#include <Engine/util/Math.hpp>

// TODO : refactorizar codigo !!!!!!

void TriggerResolutionSystem::fixedUpdate(const Context &context, float deltaTime) {
    for (const auto & message : triggerMessages) {
        if (message.type1 == PLAYER) {
            if (message.type2 == KEY) {
                // TODO : abrir puerta (con mensajes? o context?)
                auto& door = context->getEntityByID(message.ID2 - 1);
                door.removeComponent<RigidStaticAABB>();

                soundMessages.emplace_back(ACTION_GET_KEY);
                deathMessages.emplace_back(message.ID2);
            } else if (message.type2 == DOOR) {
                soundMessages.emplace_back(ACTION_OPEN_DOOR);
                deathMessages.emplace_back(message.ID2);
            }
        } else if (message.type1 == BULLET) {
        	if (message.type2 == ENEMY) {
        		auto& bullet = context->getEntityByID(message.ID1);
        		auto& character = context->getEntityByID(message.ID2);

        		auto * bulletData = bullet.getComponent<BulletData>();
        		auto * characterData = character.getComponent<CharacterData>();

		        if (characterData->mode == NEUTRAL) {
			        characterData->health -= bulletData->damage;
		        } else {
			        if (characterData->mode != bulletData->damageType)
				        characterData->health -= (bulletData->damage * FACTOR_DIFFERENT_MODE);
			        else
				        characterData->health -= (bulletData->damage * FACTOR_SAME_MODE);
		        }

		        soundMessages.emplace_back(ACTION_ENEMY_HITMARKER);

		        if (!greater_e(characterData->health, 0))
			        deathMessages.emplace_back(character.getID());
        	}
        } else if (message.type1 == ENEMY) {
        	if (message.type2 == DOOR) {
        		auto * trigger = context->getEntityByID(message.ID1).getComponent<TriggerMovSphere>();
        		trigger->position -= *trigger->velocity; // deshago el movimiento del trigger
        		*trigger->velocity = vec3(0); // pongo la velocidad a 0 para que movement system no mueva el personaje
        	}
        }
    }

    triggerMessages.clear();
}