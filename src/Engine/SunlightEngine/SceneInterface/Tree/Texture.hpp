#pragma once

#include <string_view>
#include <Engine/EngineInterface/SceneInterface/ITexture.hpp>

struct RTexture;
struct ResourceManager;

namespace Sun {
	struct Texture : ITexture {
		explicit Texture(ResourceManager *resManager, std::string_view path = std::string_view());

		~Texture() override = default;

		void render() const override;

		void setPosition(int, int) override;

		[[maybe_unused]] void setWidth(unsigned) override;

		[[maybe_unused]] void setHeight(unsigned) override;

		[[maybe_unused]] void setSize(unsigned, unsigned) override;

	private :
		RTexture * texture { nullptr };
		ResourceManager &resourceManager;
	};
}