#include <sys/MovementSystem.hpp>

void MovementSystem::fixedUpdate(const Context &context, float deltaTime) {
	// A la camara hay que darle la velocidad en funcion de la del jugador tras ser modificada por otros sistemas
	auto * camPhysics = context->getCamera().getComponent<Physics>();
	const auto * plaPhysics = context->getPlayer().getComponent<Physics>();

	camPhysics->velocity = plaPhysics->velocity;

	for (auto& cmp : context->getComponents().getComponents<Physics>()) {
		if (cmp) {
			if (cmp.getEntityType() == BULLET)
				cmp.position += cmp.velocity * deltaTime;
			else
				cmp.position += cmp.velocity;
		}
	}
}