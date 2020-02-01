#include <sys/RenderSystem.hpp>

void RenderSystem::update(const std::unique_ptr<GameContext>& context, const float deltaTime) const
{
	for (auto& node : std::get<vector<Node_ptr>>(context->getComponents(NODE_TYPE)))
		node->update(1.f);

	device.clear(&background);
	device.draw();
	device.showDrawn();
}

void RenderSystem::updateFPS(const float deltaTime) {
	++FPS_counter;

	if (greater_e((timeForFPS += deltaTime), 1.f)) {
		device.setWindowName(std::wstring(L"Cyborgeddon | FPS[" + std::to_wstring(FPS_counter) + L"]").c_str());
		timeForFPS -= 1.f;
		FPS_counter = 0;
	}
}
