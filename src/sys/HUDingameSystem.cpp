#include <sys/HUDingameSystem.hpp>
#include <util/HUDPaths.hpp>

#include <Engine/EngineInterface/SceneInterface/ITexture.hpp>

// Texturas 3 y 4 son barras de angel y demon
// si hay tiempo cambiarlo para que no sean numeros sueltos

void HUDingameSystem::fixedUpdate(const Context &context, const float deltaTime) {

	const auto & player = context->getPlayer();

	const auto * data = player.getComponent<CharacterData>();

	unsigned int ignore { 0 };

	if (data) {
		if (data->mode == DEMON)
			ignore = 3;
		else
			ignore = 4;

		auto &textures = context->getComponents().getComponents<TextureCmp>();

		const float lifebar3X = (data->health / PLAYER_HEALTH) * textures[3].texture->getSize().x;
		const float lifebar4X = (data->health / PLAYER_HEALTH) * textures[4].texture->getSize().x;

		textures[3].texture->setPosition(std::round(lifebar3X) - textures[3].texture->getSize().x ,
		                                 textures[3].texture->getPosition().y);
		textures[4].texture->setPosition(std::round(lifebar4X) - textures[4].texture->getSize().x ,
		                                 textures[4].texture->getPosition().y);

		for (unsigned int i = 0; i < NUM_INGAME_TEXTURES; ++i)
			if (i != ignore)
				textures[i].texture->render();
	}

	context->getEngine().display();
}
