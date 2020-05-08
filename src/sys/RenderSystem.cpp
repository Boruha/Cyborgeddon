#include <sys/RenderSystem.hpp>
#include <glm/gtx/string_cast.hpp>

void RenderSystem::fixedUpdate(const Context& context, float deltaTime) {
	const auto * playerPhysics = context->getPlayer().getComponent<Physics>();
	const auto & cameraNode    = context->getCamera().getComponent<Render>()->node;

	if (playerPhysics) {
		cameraNode->setTarget(playerPhysics->position);

		for (auto & cmp : context->getComponents().getComponents<Render>()) {
			if (cmp && cmp.needUpdate) {    // sin el flag needupdate el bucle dura mas del triple de tiempo de media
				cmp.node->setPosition(*cmp.pos);
				cmp.node->setRotation(*cmp.rot);
	//			cmp.node->setScale(*cmp.sca); // de momento no es necesario actualizar el escalado porque no cambia
			}
		}
	}

	const auto& iengine = context->getEngine();

	iengine.clear(BACKGROUND_COLOR, true);

	iengine.draw();
}