#include "Animation.hpp"

#include <Engine/SunlightEngine/SceneInterface/Resource/ResourceManager.hpp>

#include <filesystem>

Animation::Animation(ResourceManager * const resManager, const std::string_view path) : resourceManager(*resManager) {
	if (!path.empty())
		setAnimation(path);
}

void Animation::render(const glm::mat4 & m, Shader shader, bool visualShader) {
	model[idx]->render(m, shader, visualShader);

	idx = (idx + 1) % model.size();
}

void Animation::setAnimation(const std::string_view path) {


	unsigned int numFrames { 0 };

	for (const auto & file : std::filesystem::directory_iterator(path))
		if (file.path().extension() == ".obj")
			numFrames++;

	model.reserve(numFrames);

	for (unsigned int i = 0; i < numFrames; ++i)
		model.emplace_back(& resourceManager.getModel(std::string(path) + "/" + std::to_string(i) + ".obj"));
}
