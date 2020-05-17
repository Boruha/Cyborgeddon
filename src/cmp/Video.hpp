#pragma once

#include <cmp/Component.hpp>
#include <string>
#include <util/SoundParameter.hpp>

struct IVideo;

struct Video : public Component
{
	explicit Video(EntityType e_type, EntityID e_ID, std::string_view path);

	[[nodiscard]] constexpr static std::string_view getName() {
		return "Video";
	}

	friend std::ostream& operator<<(std::ostream&, const Video&);

	IVideo * video { nullptr };
	std::string name {};

	SoundParameter sound { NO_SOUND };

	unsigned numFrames          {0};
	unsigned frameCounter       {0};
	float    timePerFrame       {0};
	float    timeSinceLastFrame {0};

	bool     loop {false};
};