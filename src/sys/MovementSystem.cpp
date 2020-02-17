#include <sys/MovementSystem.hpp>

void MovementSystem::update(const std::unique_ptr<GameContext> &context, const float deltaTime) {
	// A la camara hay que darle la velocidad en funcion de la del jugador tras ser modificada por otros sistemas
	context->getCamera().physics->velocity = context->getPlayer().physics->velocity;

	for (auto& cmp : std::get<vector<Physics>>(context->getComponents(PHYSICS_TYPE))) {
		if (cmp) {
			if (cmp.getEntityType() == BULLET)
				cmp.position += cmp.velocity * deltaTime;
			else
				cmp.position += cmp.velocity;
		}
	}
}