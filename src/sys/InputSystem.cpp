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
			next->p_func(context->getPlayer());
		++next;
	}

	context->getPlayer().physics->velocity = context->getPlayer().velocity->direction.normalize() * context->getPlayer().velocity->speed * deltaTime;
}

void InputSystem::w_pressed(Entity& player) { ++player.velocity->direction.z; }
void InputSystem::a_pressed(Entity& player) { --player.velocity->direction.x; }
void InputSystem::s_pressed(Entity& player) { --player.velocity->direction.z; }
void InputSystem::d_pressed(Entity& player) { ++player.velocity->direction.x; }
// Dash
void InputSystem::shift_pressed(Entity& player) {
	if(player.velocity->speed == player.velocity->defaultSpeed && player.velocity->direction != 0 && CooldownSystem::dashReady())
		player.velocity->speed = 150;
}
// Shoot
void InputSystem::space_pressed(Entity& player) {
	if(player.characterData->attackingCooldown <= 0.f) player.characterData->attacking = true;
}
// Aim
void InputSystem::left_pressed  (Entity& player) { player.physics->rotation.y -= 5; }
void InputSystem::right_pressed (Entity& player) { player.physics->rotation.y += 5; }
// Switch Mode
void InputSystem::m_pressed(Entity& player) {
	player.characterData->mode = !player.characterData->mode;
	player.characterData->mode ?
		player.node->get()->setTexture("./img/textures/testing/testing_angel.jpg") :
		player.node->get()->setTexture("./img/textures/testing/testing_demon.jpg");
}