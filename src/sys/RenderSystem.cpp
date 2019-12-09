#include <sys/RenderSystem.hpp>

void RenderSystem::update()
{
	device.clear(&background);
	device.draw();
	device.showDrawn();
}