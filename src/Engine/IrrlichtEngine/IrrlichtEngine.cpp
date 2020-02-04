#include <Engine/IrrlichtEngine/IrrlichtEngine.hpp>

#include <Engine/util/Color.hpp>

#include <irrlicht/irrlicht.h>

void IrrlichtEngine::init(unsigned width, unsigned height, const wchar_t *name) {
	device = irr::createDevice(irr::video::EDT_OPENGL, irr::core::dimension2d<irr::u32>(width, height));
	device->setWindowCaption(name);
	device->setEventReceiver(&eventReceiver);

	sceneManager = device->getSceneManager();
	videoDriver  = device->getVideoDriver();
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

const Texture * IrrlichtEngine::loadTexture(const char * const path) const {
    // TODO :   de momento solo nos interesa la direccion de memoria, asi que casteamos la textura de
    //          irrlicht a void * (que es un puntero sin tipo) y a su vez lo casteamos a puntero a textura
    //          Esto simplemente es para saber que desde nuestro motor recibiremos un puntero a "Texture"
    return static_cast<Texture *>(static_cast<void *>(videoDriver->getTexture(path)));
}
