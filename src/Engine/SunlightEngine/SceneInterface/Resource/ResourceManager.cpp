#include <Engine/SunlightEngine/SceneInterface/Resource/ResourceManager.hpp>

const RModel & ResourceManager::getModel(const std::string_view path) {
	// emplace (key, value) crea el RModel value. Para construir un RModel necesitamos un parametro que en
	// este caso coincide con la clave, por eso la llamada a la funcion queda asi (despues de la condicion):
	if (model.count(path.data()) == 0)
		model.insert(std::make_pair(path.data(), RModel(path)));

	return model.find(path.data())->second;
}
