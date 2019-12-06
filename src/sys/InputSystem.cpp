#include <sys/InputSystem.hpp>

#include <iostream>

using std::cerr;
using std::endl;

int InputSystem::init(irr::IrrlichtDevice *device) {
	device->setEventReceiver(&eventReceiver);

	if (!device->getEventReceiver()) {
		cerr << "Error en input system init, no se pudo establecer el receptor de eventos" << endl;
		return 1;
	}

	return 0;
}

void InputSystem::update(EntityPlayer &player) {
	player.velocity.velocity.x = player.velocity.velocity.z = 0;
	auto* next = const_cast<TKey2func*>(mapping);

	while(next->p_func)
	{
		if(eventReceiver.IsKeyDown(next->key))
			next->p_func(player);
		++next;
	}
}
