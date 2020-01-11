#include <sys/RenderSystem.hpp>

// TODO: poder dibujar las cosas individualmente de alguna manera ( prescindir de sceneManager->drawAll() )
void RenderSystem::update(const std::unique_ptr<GameContext> &context, const float deltaTime) const
{
	for (auto& node : context->getNodeComponents())
		node->update(deltaTime);

	device.clear(&background);
	device.draw();
	device.showDrawn();
}