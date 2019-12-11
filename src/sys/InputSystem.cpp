#include <sys/InputSystem.hpp>

#include <src/man/EntityManager.hpp>
#include <algorithm>

using std::cerr;
using std::endl;

void InputSystem::init() {
	device.setEventReceiver(&eventReceiver);
}

// TODO: revisar los punteros a funcion. Problema -> distintos parametros para distintas acciones
void InputSystem::update(const std::vector<std::unique_ptr<EntityPlayer>>& players) {
	for (auto &player : players) {
		player->velocity.velocity.x = player->velocity.velocity.z = player->velocity.direction.x = player->velocity.direction.z = 0;

		auto *next = const_cast<TKey2func *>(keyMapping);

		while (next->p_func) {
			if (eventReceiver.IsKeyDown(next->key))
				next->p_func(*player);
			++next;
		}
	}
}