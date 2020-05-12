#include <sys/HUDpauseSystem.hpp>
#include <util/HUDPaths.hpp>

#include <Engine/EngineInterface/SceneInterface/ITexture.hpp>

void HUDpauseSystem::fixedUpdate(const Context &context, const float deltaTime) {
	context->getEngine().clear(Color(BLACK), true);

	auto& menu_option  = context->getComponents().getComponents<MenuOption>()[0];

	auto * currentTexture = & context->getComponents().getComponents<TextureCmp>()[NUM_INGAME_TEXTURES + menu_option.option];

	currentTexture->texture->render();

	currentTexture = & context->getComponents().getComponents<TextureCmp>()[NUM_INGAME_TEXTURES + menu_option.maxOptions];

	currentTexture->texture->render();

	context->getEngine().display();
}
