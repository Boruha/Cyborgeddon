#pragma once

#include <Engine/SunlightEngine/SceneInterface/Tree/IEntity.hpp>

#include <vector>
#include <string_view>

struct RModel;

struct Animation : IEntity {
	explicit Animation(ResourceManager * resManager, std::string_view path = std::string_view());

	~Animation() override = default;

	void render(const glm::mat4& m, Shader shader, bool visualShader) override;

	void setAnimation(std::string_view path);

private :
	std::vector <const RModel *> model {};
	ResourceManager& resourceManager;
	unsigned int idx {0};
};