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
	player.vDirection.X = player.vDirection.Z = 0;

	// Pulsamos W
	if (eventReceiver.IsKeyDown(irr::KEY_KEY_W) && !eventReceiver.IsKeyDown(irr::KEY_KEY_S)) {
		player.vDirection.Z = 1;
	}
	// Pulsamos S
	else if (!eventReceiver.IsKeyDown(irr::KEY_KEY_W) && eventReceiver.IsKeyDown(irr::KEY_KEY_S)) {
		player.vDirection.Z = -1;
	}

	// Pulsamos D
	if (eventReceiver.IsKeyDown(irr::KEY_KEY_D) && !eventReceiver.IsKeyDown(irr::KEY_KEY_A)) {
		player.vDirection.X = 1;
	}
	// Pulsamos A
	else if (!eventReceiver.IsKeyDown(irr::KEY_KEY_D) && eventReceiver.IsKeyDown(irr::KEY_KEY_A)) {
		player.vDirection.X = -1;
	}
	
	//std::cout << "vDirection pre nomalice: " << player.vDirection.X << std::endl;
	player.vDirection.normalize();
	//std::cout << "vDirection post nomalice: " << player.vDirection.X << std::endl;

}