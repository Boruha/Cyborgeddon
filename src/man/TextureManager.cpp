#include <man/TextureManager.hpp>

#include <util/TexturePaths.hpp>

#include <SunlightEngine/Device.hpp>

#include <Engine/EngineInterface/Engine.hpp>

void TextureManager::loadTextures() const {
    device.getInnerDevice()->getVideoDriver()->getTexture(WALL_TEXTURE);
    device.getInnerDevice()->getVideoDriver()->getTexture(DOOR_TEXTURE);
    device.getInnerDevice()->getVideoDriver()->getTexture(KEY_TEXTURE);
    device.getInnerDevice()->getVideoDriver()->getTexture(DEMON_TEXTURE);
    device.getInnerDevice()->getVideoDriver()->getTexture(ANGEL_TEXTURE);
    device.getInnerDevice()->getVideoDriver()->getTexture(ENEMY_TEXTURE);
    device.getInnerDevice()->getVideoDriver()->getTexture(CONTROLS_TEXTURE);
    device.getInnerDevice()->getVideoDriver()->getTexture(TIPS_TEXTURE);
}

/*
void TextureManager::loadTextures(const Engine * const engine) const {
    engine->loadTexture(WALL_TEXTURE);
    engine->loadTexture(DOOR_TEXTURE);
    engine->loadTexture(KEY_TEXTURE);
    engine->loadTexture(DEMON_TEXTURE);
    engine->loadTexture(ANGEL_TEXTURE);
    engine->loadTexture(ENEMY_TEXTURE);
    engine->loadTexture(CONTROLS_TEXTURE);
    engine->loadTexture(TIPS_TEXTURE);
}
*/