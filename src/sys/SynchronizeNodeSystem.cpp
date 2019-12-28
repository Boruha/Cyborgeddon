#include <sys/SynchronizeNodeSystem.hpp>

void SynchronizeNodeSystem::update(const std::unique_ptr<GameContext>& context) const {
	context->getCamera()->node->update();

	for (const auto& node : context->getSceneNodeComponents())
		node.update();
}