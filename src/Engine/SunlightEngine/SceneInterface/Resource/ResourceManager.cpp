#include <Engine/SunlightEngine/SceneInterface/Resource/ResourceManager.hpp>

const RModel & ResourceManager::getModel(const std::string_view path) {
	// emplace (key, value) crea el RModel value. Para construir un RModel necesitamos un parametro que en
	// este caso coincide con la clave, por eso la llamada a la funcion queda asi (despues de la condicion):
	if (model.count(path.data()) == 0)
		return model.emplace(
				std::piecewise_construct,
				std::forward_as_tuple(path.data()),
				std::forward_as_tuple(path)
		).first->second;

	return model.find(path.data())->second;
}

RVideo & ResourceManager::getVideo(const std::string_view path) {
	if (video.count(path.data()) == 0) {
		return video.emplace(
				std::piecewise_construct,
				std::forward_as_tuple(path.data()),
				std::forward_as_tuple(path)
		).first->second;
	}

	return video.find(path.data())->second;
}

RTexture & ResourceManager::getTexture(const std::string_view path) {
	if (texture.count(path.data()) == 0) {
		return texture.emplace(
				std::piecewise_construct,
				std::forward_as_tuple(path.data()),
				std::forward_as_tuple(path)
		).first->second;
	}

	return texture.find(path.data())->second;
}

void ResourceManager::removeModel(const std::string_view path) {
	model.erase(path.data());
}

void ResourceManager::removeVideo(const std::string_view path) {
	video.erase(path.data());
}

void ResourceManager::removeTexture(const std::string_view path) {
	texture.erase(path.data());
}

void ResourceManager::removeAllResources(){
	model.clear();
}