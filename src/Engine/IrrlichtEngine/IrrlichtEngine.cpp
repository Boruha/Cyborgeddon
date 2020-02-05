#include <Engine/IrrlichtEngine/IrrlichtEngine.hpp>

#include <Engine/util/Color.hpp>

#include <Engine/IrrlichtEngine/SceneInterface/IrrlichtObjectNode.hpp>
#include <Engine/IrrlichtEngine/SceneInterface/IrrlichtCameraNode.hpp>

#include <irrlicht/irrlicht.h>

void IrrlichtEngine::init(unsigned width, unsigned height, const wchar_t *name) {
	device = irr::createDevice(irr::video::EDT_OPENGL, irr::core::dimension2d<irr::u32>(width, height));
	device->setWindowCaption(name);
	device->setEventReceiver(&eventReceiver);

	sceneManager = device->getSceneManager();
	videoDriver  = device->getVideoDriver();

	scene = std::make_unique<IrrlichtScene>(sceneManager);
}

void IrrlichtEngine::shutdown() const {
	device->drop();
}

bool IrrlichtEngine::run() const {
	return device->run();
}

bool IrrlichtEngine::isKeyPressed(const KEY_CODE code) const {
	return eventReceiver.IsKeyDown(code);
}

const Mouse & IrrlichtEngine::getMouse() const {
	return eventReceiver.getMouse();
}

void IrrlichtEngine::clear(const Color color) const {
	videoDriver->beginScene(true, true, irr::video::SColor(color.a, color.r, color.g, color.b));
}

void IrrlichtEngine::draw() const {
	sceneManager->drawAll();
}

void IrrlichtEngine::display() const {
	videoDriver->endScene();
}