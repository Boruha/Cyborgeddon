#include <sys/RenderSystem.hpp>

// TODO: poder dibujar las cosas individualmente de alguna manera ( prescindir de sceneManager->drawAll() )
void RenderSystem::update(const std::unique_ptr<GameContext> &context, const float deltaTime) const
{
	for (auto& node : std::get<vector<Node_ptr>>(context->getComponents(NODE_TYPE))) {
		node->update(deltaTime);
	}

	device.clear(&background);
	device.draw();
	device.showDrawn();
}