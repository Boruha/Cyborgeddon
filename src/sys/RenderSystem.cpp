#include <sys/RenderSystem.hpp>
#include <Engine/util/Math.hpp>
#include <glm/gtx/string_cast.hpp>

void RenderSystem::update(const Context& context, const float deltaTime) {

	auto& player = context->getPlayer();
	auto& camera = context->getCamera();

	camera.getComponent<Render>()->node->setTarget(player.getComponent<Physics>()->position);

	for (auto & cmp : context->getComponents().get<Render>()) {
		if (cmp) {
			cmp.node->setPosition(*cmp.pos);
			cmp.node->setRotation(*cmp.rot);
			cmp.node->setScale(*cmp.sca);
		}
	}

	engine->clear(Color(background));
	engine->draw();
	engine->display();
}