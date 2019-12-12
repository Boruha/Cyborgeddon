#include <sys/RenderSystem.hpp>

// TODO: poder dibujar las cosas individualmente de alguna manera ( prescindir de sceneManager->drawAll() )
void RenderSystem::update()
{
	device.clear(&background);
	device.draw();
	device.showDrawn();
}