#include <sys/SynchronizeNodeSystem.hpp>

void SynchronizeNodeSystem::update(const std::unique_ptr<GameContext>& context) const {
	for (const auto& node : context->getNodeComponents())
		node->update();
}