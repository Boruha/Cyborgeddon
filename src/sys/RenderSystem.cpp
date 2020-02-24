#include <sys/RenderSystem.hpp>
#include <Engine/util/Math.hpp>

void RenderSystem::update(const Context& context, const float deltaTime) {
	for (const auto& node : context->getNodes())
		if (*node)
			node->update(1.f);

	engine->clear(Color(background));
	engine->draw();
	engine->display();
}