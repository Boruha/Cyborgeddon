#include <sys/RenderSystem.hpp>

int RenderSystem::init()
{
	return 0;
}

void RenderSystem::update()
{
	device->clear(Sun::Color(255,255,255));
	device->draw();
	device->showDrawn();
}