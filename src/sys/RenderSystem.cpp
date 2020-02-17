#include <sys/RenderSystem.hpp>
#include <Engine/util/Math.hpp>

void RenderSystem::update(const std::unique_ptr<GameContext>& context, const float deltaTime) {
	for (const auto& node : std::get<vector<std::unique_ptr<INode>>>(context->getComponents(INODE_TYPE)))
		if (*node)
			node->update(1.f);

	engine->clear(Color(background));
	engine->draw();
	engine->display();
}