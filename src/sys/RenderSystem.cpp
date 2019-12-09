#include <sys/RenderSystem.hpp>

void RenderSystem::update()
{
	device.clear(Sun::Color(255,255,255));
	device.draw();
	device.showDrawn();
}