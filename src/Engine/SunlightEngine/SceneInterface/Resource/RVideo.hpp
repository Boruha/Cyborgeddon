#pragma once

#include <string_view>

#include <Engine/SunlightEngine/SceneInterface/Resource/IResource.hpp>

#include <opencv2/opencv.hpp>
#include <src/Engine/util/shaders/Shader.hpp>
#include <Engine/util/shaders/ShaderPath.hpp>

struct RVideo : IResource {
	explicit RVideo(std::string_view);
	~RVideo() override;

	void render() const;

	void reset();

	void nextFrame();

	double getNumFrames() { return capture.get(CV_CAP_PROP_FRAME_COUNT); }

	double getCurrentFrame() { return capture.get(CV_CAP_PROP_POS_FRAMES); };

	double getFPS() { return capture.get(CV_CAP_PROP_FPS); };

	void setLoop(const bool loop) { loopVideo = loop; }


	private :
		cv::VideoCapture capture {};
		cv::Mat frame {};

		unsigned FBO {}, VAO {}, VBO {};

		Shader shader { TEXTURE_SHADERS };

		bool loopVideo = false;

	[[nodiscard]] unsigned getTextureFromFrame() const;
};