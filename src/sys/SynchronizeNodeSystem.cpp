#include <sys/SynchronizeNodeSystem.hpp>

void SynchronizeNodeSystem::update(const std::unique_ptr<GameContext>& context) const {
	for (const auto& node : context->getCameraNodeComponents())
		node.update();

	for (const auto& node : context->getSceneNodeComponents())
		node.update();
}