#include <Engine/SunlightEngine/SunlightEngine.hpp>

#include <Engine/util/Color.hpp>
#include <Engine/util/Mouse.hpp>

void SunlightEngine::init(unsigned width, unsigned height, const wchar_t *name) {

}

bool SunlightEngine::run() const {
	return false;
}

void SunlightEngine::shutdown() const {

}

bool SunlightEngine::isKeyPressed(const KEY_CODE code) const {
	return false;
}

const Mouse &SunlightEngine::getMouse() const {
	return *(new Mouse());
}

void SunlightEngine::clear(const Color color) const {

}

void SunlightEngine::draw() const {

}

void SunlightEngine::display() const {

}
