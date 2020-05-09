#include <sys/HUDingameSystem.hpp>

#include <Engine/EngineInterface/SceneInterface/ITexture.hpp>

void HUDingameSystem::fixedUpdate(const Context &context, const float deltaTime) {

	for (const auto & texture : context->getComponents().getComponents<TextureCmp>())
		texture.texture->render();

	context->getEngine().display();
}
