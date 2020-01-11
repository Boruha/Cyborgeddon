#include <sys/MovementSystem.hpp>
//#include <cmath>

void MovementSystem::update(const std::unique_ptr<GameContext> &context, const float deltaTime) const {
	// A la camara hay que darle la velocidad en funcion de la del jugador tras ser modificada por otros sistemas
	context->getCamera().physics->velocity = context->getPlayer().physics->velocity;

	for (auto& cmp : context->getPhysicsComponents()) {
		if (cmp.getEntityType() != UNDEFINED) {
			cmp.position += cmp.velocity;
	//		cmp.rotation.y = fmodf(cmp.rotation.y, 360);
		}
	}
}