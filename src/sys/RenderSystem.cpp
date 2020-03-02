#include <sys/RenderSystem.hpp>

void RenderSystem::update(const Context& context, const float deltaTime) {

	auto & player   	= context->getPlayer();
	auto * cameraNode 	= context->getCamera().getComponent<Render>()->node;

	cameraNode->setTarget(player.getComponent<Physics>()->position);

	for (auto & cmp : context->getComponents().get<Render>()) {
		if (cmp && cmp.needUpdate) {    // sin el flag needupdate el bucle dura mas del triple de tiempo de media
			cmp.node->setPosition(*cmp.pos);
			cmp.node->setRotation(*cmp.rot);
//			cmp.node->setScale(*cmp.sca); // de momento no es necesario actualizar el escalado porque no cambia
		}
	}

	engine->clear(Color(background));
	engine->draw();
	engine->display();
}