#include <man/TextureManager.hpp>

#include <util/TexturePaths.hpp>

#include <SunlightEngine/Device.hpp>

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
