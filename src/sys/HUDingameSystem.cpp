#include <sys/HUDingameSystem.hpp>
#include <util/HUDPaths.hpp>

#include <Engine/EngineInterface/SceneInterface/ITexture.hpp>

// Texturas 3 y 4 son barras de angel y demon

void HUDingameSystem::fixedUpdate(const Context &context, const float deltaTime) {

	auto& player = context->getPlayer();

	auto* data = player.getComponent<CharacterData>();

	unsigned int ignore { 0 };

	if (data) {
		if (data->mode == DEMON)
			ignore = 3;
		else
			ignore = 4;

		auto &textures = context->getComponents().getComponents<TextureCmp>();

		int lifebar3X = (PLAYER_HEALTH / data->health) * textures[3].texture->getSize().x;
		int lifebar4X = (PLAYER_HEALTH / data->health) * textures[4].texture->getSize().x;

		textures[3].texture->setPosition(textures[3].texture->getSize().x - lifebar3X,
		                                 textures[3].texture->getPosition().y);
		textures[4].texture->setPosition(textures[4].texture->getSize().x - lifebar4X,
		                                 textures[4].texture->getPosition().y);

		for (unsigned int i = 0; i < NUM_INGAME_TEXTURES; ++i)
			if (i != ignore)
				textures[i].texture->render();
	}

	context->getEngine().display();
}
