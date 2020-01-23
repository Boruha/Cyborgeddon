#include <sys/InputSystem.hpp>

#include <src/man/EntityManager.hpp>

void InputSystem::init() {
	device.setEventReceiver(&eventReceiver);
}

// TODO: revisar los punteros a funcion. Problema -> distintos parametros para distintas acciones
//  	 posible solucion: usar gamecontext para lo necesario en cada funcion
void InputSystem::update(const std::unique_ptr<GameContext> &context, const float deltaTime) const {
	context->getPlayer().velocity->direction = 0;

	auto* next = const_cast<TKey2func*>(keyMapping);

	while (next->p_func) {
		if (eventReceiver.IsKeyDown(next->key))
			next->p_func(context->getPlayer(), deltaTime);
		++next;
	}

	context->getPlayer().physics->velocity = context->getPlayer().velocity->direction.normalize() * context->getPlayer().velocity->currentSpeed * deltaTime;
}

void InputSystem::w_pressed(Entity& player, const float deltaTime) { ++player.velocity->direction.z; }
void InputSystem::a_pressed(Entity& player, const float deltaTime) { --player.velocity->direction.x; }
void InputSystem::s_pressed(Entity& player, const float deltaTime) { --player.velocity->direction.z; }
void InputSystem::d_pressed(Entity& player, const float deltaTime) { ++player.velocity->direction.x; }
// Dash
void InputSystem::shift_pressed(Entity& player, const float deltaTime) {
	if(player.velocity->currentSpeed == player.velocity->defaultSpeed && player.velocity->direction != 0 && CooldownSystem::dashReady())
		player.velocity->currentSpeed = 600;
}
// Shoot
void InputSystem::space_pressed(Entity& player, const float deltaTime) {
	if(player.characterData->currentAttackingCooldown <= 0.f) player.characterData->attacking = true;
}
// Aim
void InputSystem::left_pressed  (Entity& player, const float deltaTime) { player.physics->rotation.y -= 270 * deltaTime; }
void InputSystem::right_pressed (Entity& player, const float deltaTime) { player.physics->rotation.y += 270 * deltaTime; }
// Switch Mode
void InputSystem::m_pressed(Entity& player, const float deltaTime) {
	player.characterData->mode = !player.characterData->mode;
	player.characterData->mode ?
		player.node->get()->setTexture(ANGEL_TEXTURE) :
		player.node->get()->setTexture(DEMON_TEXTURE);
}