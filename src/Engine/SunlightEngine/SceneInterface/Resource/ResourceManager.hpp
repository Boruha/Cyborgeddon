#pragma once

#include <unordered_map>

#include <Engine/SunlightEngine/SceneInterface/Resource/RModel.hpp>

struct ResourceManager {
	const RModel& getModel(std::string_view path);

	private:
		std::unordered_map<std::string, RModel> model;
};