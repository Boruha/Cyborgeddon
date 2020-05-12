#include <sys/HUDingameSystem.hpp>
#include <util/HUDPaths.hpp>

#include <Engine/EngineInterface/SceneInterface/ITexture.hpp>

void HUDingameSystem::fixedUpdate(const Context &context, const float deltaTime) {

	for (unsigned int i = 0; i < NUM_INGAME_TEXTURES; ++i)
		context->getComponents().getComponents<TextureCmp>()[i].texture->render();

	context->getEngine().display();
}
