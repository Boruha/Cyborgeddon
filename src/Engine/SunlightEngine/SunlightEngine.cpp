#include <Engine/SunlightEngine/SunlightEngine.hpp>

#include <Engine/util/Color.hpp>
#include <Engine/util/Mouse.hpp>

void SunlightEngine::init(const unsigned width, const unsigned height, const wchar_t * const name) {

}

bool SunlightEngine::run() const {
	return false;
}

void SunlightEngine::shutdown() const {

}

bool SunlightEngine::isKeyPressed(const KEY_CODE code) const {
	return false;
}

const Mouse & SunlightEngine::getMouse() const {
	// TODO : 	CAMBIAR EL VALOR DE RETORNO
	//  			vamos a hacer que el juego acabe para evitar que se nos pase actualizar este metodo
	exit(-1);
	return *(new Mouse());
}

void SunlightEngine::clear(const Color color) const {

}

void SunlightEngine::draw() const {

}

void SunlightEngine::display() const {

}