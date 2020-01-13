#include <sys/CharacterDataSystem.hpp>

void CharacterDataSystem::update(const std::unique_ptr<GameContext>& context, const float deltaTime) const {
	for (const auto& data : context->getCharacterDataComponents())
		if (data.getEntityType() != UNDEFINED)
			if (data.health <= 0)
				context->addToDestroy(data.getEntityID());
}
