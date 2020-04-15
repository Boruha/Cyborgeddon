#pragma once

#include <string_view>

#include <Engine/SunlightEngine/SceneInterface/Resource/IResource.hpp>

#include <GL/glew.h>

#include <Engine/util/Error.hpp>

#include <opencv2/opencv.hpp>

struct RVideo : IResource {
	explicit RVideo(std::string_view);
	~RVideo() override;

	void render() const;

	private :
		cv::VideoCapture capture {};
		cv::Mat frame {};

		unsigned FBO {};

	[[nodiscard]] unsigned getTextureFromFrame() const;
};