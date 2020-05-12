#include <sys/HUDintroSystem.hpp>

#include <Engine/EngineInterface/SceneInterface/ITexture.hpp>


void HUDintroSystem::fixedUpdate(const Context &context, const float deltaTime) {
	context->getEngine().clear(Color(BLACK), true);

	if (context->getVideoIndex() > 0) {
		auto& menu_option  = context->getComponents().getComponents<MenuOption>()[0];

		auto& texture = context->getComponents().getComponents<TextureCmp>()[menu_option.option];

		texture.texture->render();
	}
}
