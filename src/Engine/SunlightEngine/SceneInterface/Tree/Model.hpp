#pragma once

#include <Engine/SunlightEngine/SceneInterface/Tree/IEntity.hpp>

#include <Engine/SunlightEngine/SceneInterface/Resource/RModel.hpp>

struct Model : IEntity {
	explicit Model(ResourceManager * resManager, std::string_view path = std::string_view());

    ~Model() override = default;

    void render(const glm::mat4& m, Shader shader, bool visualShader) override;

    void setModel(std::string_view path);

    private :
		const RModel * model { nullptr };
		ResourceManager& resourceManager;
};