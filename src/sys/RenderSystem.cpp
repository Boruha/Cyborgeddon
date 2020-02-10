#include <sys/RenderSystem.hpp>
#include <Engine/util/Math.hpp>

void RenderSystem::update(const std::unique_ptr<GameContext>& context, const float deltaTime) const
{
	for (const auto& node : std::get<vector<Node_ptr>>(context->getComponents(NODE_TYPE)))
		node->update(1.f);

	device.clear(&background);
	device.draw();
	device.showDrawn();
}

void RenderSystem::updateFPS(const float deltaTime) {
	++FPS_counter;

	if (greater_e((timeForFPS += deltaTime), 1.f)) {
		device.setWindowName(std::wstring(GAME_NAME + std::wstring(L" | FPS[") + std::to_wstring(FPS_counter) + std::wstring(L"]")).c_str());
		timeForFPS -= 1.f;
		FPS_counter = 0;
	}
}
