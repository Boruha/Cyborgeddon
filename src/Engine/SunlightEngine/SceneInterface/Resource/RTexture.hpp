#pragma once

#include <string_view>

#include <Engine/SunlightEngine/SceneInterface/Resource/IResource.hpp>

#include <Engine/SunlightEngine/SceneInterface/Resource/Texture.hpp>

#include <Engine/util/shaders/Shader.hpp>
#include <Engine/util/shaders/ShaderPath.hpp>

#include <glm/glm.hpp>

struct RTexture : IResource {
	explicit RTexture(std::string_view);
	~RTexture() override;

	void render() const;

	void setPosition(int, int);
	glm::vec2 getPosition() const;

	[[maybe_unused]] void setWidth(unsigned);
	[[maybe_unused]] void setHeight(unsigned);

	[[maybe_unused]] void setSize(unsigned, unsigned);

	[[nodiscard]] glm::vec2 getSize() const { return { width, height }; }

	private:

		void setupTexture() const;

		unsigned int VAO {0}, VBO{0};
		int x{0}, y{0}, width{0}, height{0};

		Texture texture {};

		Shader shader { TEXTURE_SHADERS };
};