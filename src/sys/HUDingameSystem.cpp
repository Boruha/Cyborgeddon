#include <sys/HUDingameSystem.hpp>
#include <util/HUDPaths.hpp>

#include <Engine/EngineInterface/SceneInterface/ITexture.hpp>

// Texturas 3 y 4 son barras de angel y demon
// si hay tiempo cambiarlo para que no sean numeros sueltos

void HUDingameSystem::fixedUpdate(const Context &context, const float deltaTime) {

	const auto & player = context->getPlayer();

	const auto * data = player.getComponent<CharacterData>();

	unsigned int ignoreLifebar  { 0 }; //Cambio barra vida Angel/Demon
	unsigned int ignoreKeyDemon { 0 }; //Llave Demon cogida o no
    unsigned int ignoreKeyAngel { 0 }; //Llave Angel cogida o no
	if (data) {
		if (data->mode == DEMON)
			ignoreLifebar = 3;
		else
			ignoreLifebar = 4;

		//Comprobamos si tenemos la llave o no
		auto& llaves = context->getComponents().getComponents<Lock>();

		if(!llaves.at(0).checks[0]) //Llave demon
            ignoreKeyDemon = 0;
		else
            ignoreKeyDemon = 10;


        if(!llaves.at(0).checks[1]) //Llave angel
            ignoreKeyAngel = 1;
        else
            ignoreKeyAngel = 10;

		auto &textures = context->getComponents().getComponents<TextureCmp>();

		const float lifebar3X = (data->health / PLAYER_HEALTH) * textures[3].texture->getSize().x;
		const float lifebar4X = (data->health / PLAYER_HEALTH) * textures[4].texture->getSize().x;

		textures[3].texture->setPosition(std::round(lifebar3X) - textures[3].texture->getSize().x ,
		                                 textures[3].texture->getPosition().y);
		textures[4].texture->setPosition(std::round(lifebar4X) - textures[4].texture->getSize().x ,
		                                 textures[4].texture->getPosition().y);

		for (unsigned int i = 0; i < NUM_INGAME_TEXTURES; ++i)
			if (i != ignoreLifebar && i != ignoreKeyDemon && i != ignoreKeyAngel)
				textures[i].texture->render();
	}

	context->getEngine().display();
}
