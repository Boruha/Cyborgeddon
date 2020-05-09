#include "Model.hpp"

#include <Engine/SunlightEngine/SceneInterface/Resource/ResourceManager.hpp>

Model::Model(ResourceManager * const resManager, const std::string_view path) : resourceManager(*resManager) {
	if (!path.empty())
		setModel(path);
}

void Model::render(const glm::mat4 & m, Shader shader, bool visualShader) {
	model->render(m, shader, visualShader);
}

void Model::setModel(const std::string_view path) {
	model = &resourceManager.getModel(path);
}
