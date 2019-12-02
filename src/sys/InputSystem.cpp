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

void InputSystem::update(EntityPlayer& player) {
	player.velocity.velocity.X = player.velocity.velocity.Z = 0;

	// Pulsamos W
	if (eventReceiver.IsKeyDown(irr::KEY_KEY_W) && !eventReceiver.IsKeyDown(irr::KEY_KEY_S)) {
		player.velocity.velocity.Z = 1;
	}
	// Pulsamos S
	else if (!eventReceiver.IsKeyDown(irr::KEY_KEY_W) && eventReceiver.IsKeyDown(irr::KEY_KEY_S)) {
		player.velocity.velocity.Z = -1;
	}

	// Pulsamos D
	if (eventReceiver.IsKeyDown(irr::KEY_KEY_D) && !eventReceiver.IsKeyDown(irr::KEY_KEY_A)) {
		player.velocity.velocity.X = 1;
	}
	// Pulsamos A
	else if (!eventReceiver.IsKeyDown(irr::KEY_KEY_D) && eventReceiver.IsKeyDown(irr::KEY_KEY_A)) {
		player.velocity.velocity.X = -1;
	}
}