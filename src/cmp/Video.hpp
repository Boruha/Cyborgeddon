#pragma once

#include <cmp/Component.hpp>
#include <Engine/EngineInterface/SceneInterface/IVideo.hpp>
#include <string>
#include <memory>
#include <iostream>

struct Video : public Component
{
	explicit Video(EntityType e_type, EntityID e_ID, std::string_view path);

	[[nodiscard]] constexpr static std::string_view getName() {
		return "Video";
	}

	friend std::ostream& operator<<(std::ostream&, const Video&);

	IVideo * video { nullptr };
	std::string name {};
};