#pragma once

#include <cmp/Component.hpp>

struct ITexture;

struct TextureCmp : Component {
	explicit TextureCmp(const EntityType e_type, const EntityID e_ID)
			: Component(e_type, e_ID) {  }

	[[nodiscard]] constexpr static std::string_view getName() {
		return "TextureCmp";
	}

	friend std::ostream& operator<<(std::ostream& os, const TextureCmp& cmp);

	ITexture * texture { nullptr };
};