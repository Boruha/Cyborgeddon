#include <sys/CharacterDataSystem.hpp>

void CharacterDataSystem::update(const std::unique_ptr<GameContext>& context, const float deltaTime) const {
	for (const auto& data : std::get<vector<CharacterData>>(context->getComponents(CHARACTER_DATA_TYPE)))
		if (data.getEntityType() != UNDEFINED)
			if (!Sun::greater_e(data.health, 0))
				context->addToDestroy(data.getEntityID());
}
