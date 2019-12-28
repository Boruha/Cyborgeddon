#include <sys/InputSystem.hpp>

#include <src/man/EntityManager.hpp>

void InputSystem::init() {
	device.setEventReceiver(&eventReceiver);
}

// TODO: revisar los punteros a funcion. Problema -> distintos parametros para distintas acciones
//  	 posible solucion: usar gamecontext para lo necesario en cada funcion
void InputSystem::update(const std::unique_ptr<GameContext>& context) const {
	context->getPlayer()->velocity->direction = 0;

	auto* next = const_cast<TKey2func*>(keyMapping);

	while (next->p_func) {
		if (eventReceiver.IsKeyDown(next->key))
			next->p_func(context->getPlayer());
		++next;
	}

	context->getPlayer()->velocity->velocity = context->getPlayer()->velocity->direction * context->getPlayer()->velocity->speed;
}

void InputSystem::w_pressed(std::unique_ptr<EntityPlayer>& player) { ++player->velocity->direction.z; }
void InputSystem::a_pressed(std::unique_ptr<EntityPlayer>& player) { --player->velocity->direction.x; }
void InputSystem::s_pressed(std::unique_ptr<EntityPlayer>& player) { --player->velocity->direction.z; }
void InputSystem::d_pressed(std::unique_ptr<EntityPlayer>& player) { ++player->velocity->direction.x; }
// Dash
void InputSystem::shift_pressed(std::unique_ptr<EntityPlayer>& player) {
	if(player->velocity->speed <= 1 && player->velocity->direction != 0 && CooldownSystem::dashReady())
		player->velocity->speed = 16;
}
// Shoot
void InputSystem::space_pressed(std::unique_ptr<EntityPlayer>& player) {
	if(CooldownSystem::shootReady()) player->shooting = true;
}
// Aim
void InputSystem::left_pressed  (std::unique_ptr<EntityPlayer>& player) { --player->transformable->rotation.y; }
void InputSystem::right_pressed (std::unique_ptr<EntityPlayer>& player) { ++player->transformable->rotation.y; }
// Switch Mode
void InputSystem::m_pressed(std::unique_ptr<EntityPlayer>& player) {
	player->mode = !player->mode;
	player->mode ?
		player->node->setTexture("./img/textures/testing/testing_angel.jpg") :
		player->node->setTexture("./img/textures/testing/testing_demon.jpg");
}