#include <sys/InputSystem.hpp>

#include <src/man/EntityManager.hpp>

// TODO: revisar los punteros a funcion. Problema -> distintos parametros para distintas acciones
//  	 posible solucion: usar gamecontext para lo necesario en cada funcion
void InputSystem::update(const std::unique_ptr<GameContext>& context) const {
	context->getPlayer()->velocity.direction = 0;

	auto* next = const_cast<TKey2func *>(keyMapping);

	while (next->p_func) {
		if (eventReceiver.IsKeyDown(next->key))
			next->p_func(context->getPlayer());
		++next;
	}
}