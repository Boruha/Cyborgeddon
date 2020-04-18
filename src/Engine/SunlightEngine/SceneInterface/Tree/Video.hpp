#pragma once

#include <string_view>
#include <Engine/EngineInterface/SceneInterface/IVideo.hpp>

struct RVideo;
struct ResourceManager;

struct Video : IVideo {

	explicit Video(ResourceManager * resManager, std::string_view path = std::string_view());

	~Video() override = default;

	void setVideo(std::string_view);

	void render() const override;

	void reset() override;

	void nextFrame() override;

	double getCurrentFrame() override;

	double getNumFrames() override;

	double getFPS() override;

	void setLoop(bool) override;

	private :
		RVideo * video { nullptr };
		ResourceManager& resourceManager;
};