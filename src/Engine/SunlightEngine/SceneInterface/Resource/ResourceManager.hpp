#pragma once

#include <unordered_map>

#include <Engine/SunlightEngine/SceneInterface/Resource/RVideo.hpp>
#include <Engine/SunlightEngine/SceneInterface/Resource/RModel.hpp>

struct ResourceManager {
	const   RModel& getModel(std::string_view);
			RVideo& getVideo(std::string_view);

	void removeModel(std::string_view);
	void removeVideo(std::string_view);

	private:
		std::unordered_map<std::string, RModel> model {};
		std::unordered_map<std::string, RVideo> video {};
};