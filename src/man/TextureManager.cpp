#include <man/TextureManager.hpp>

#include <util/TexturePaths.hpp>

#include <SunlightEngine/Device.hpp>

#include <Engine/EngineInterface/IEngine.hpp>

void TextureManager::loadTextures() const {
    engine->scene->loadTexture(WALL_TEXTURE);
    engine->scene->loadTexture(DOOR_TEXTURE);
    engine->scene->loadTexture(KEY_TEXTURE);
    engine->scene->loadTexture(DEMON_TEXTURE);
    engine->scene->loadTexture(ANGEL_TEXTURE);
    engine->scene->loadTexture(ENEMY_TEXTURE);
    engine->scene->loadTexture(CONTROLS_TEXTURE);
    engine->scene->loadTexture(TIPS_TEXTURE);
}

void TextureManager::unloadTextures() const {
	engine->scene->unloadTextures();
}