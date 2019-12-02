#include <sys/RenderSystem.hpp>
#include <irrlicht/irrlicht.h>

#include <iostream>

using std::cerr;
using std::endl;

int RenderSystem::init()
{
	if(!device)
	{
		cerr << "Error en render system init, no se crea el device" << endl;
		return 1;
	}

	device->setWindowCaption(L"Cyborgeddon");

	return 0;
}

void RenderSystem::update()
{
	device->getVideoDriver()->beginScene(true, true, irr::video::SColor(255, 255, 255, 255));
	device->getSceneManager()->drawAll();
	device->getVideoDriver()->endScene();
}