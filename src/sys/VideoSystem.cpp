#include <sys/VideoSystem.hpp>
#include <Engine/EngineInterface/SceneInterface/IVideo.hpp>
#include <Engine/util/Math.hpp>
#include <Engine/util/Timer.hpp>

void VideoSystem::update(const Context &context, const float deltaTime) {

	if (context->getComponents().getComponents<Video>().size() > context->getVideoIndex()) {

		auto& video = context->getComponents().getComponents<Video>()[context->getVideoIndex()];

		if (!less_e((std::chrono::high_resolution_clock::now() - time).count() / 1000000000.f, video.timePerFrame)) {

			time = std::chrono::high_resolution_clock::now();

			if (video.frameCounter == 0)
				soundMessages.emplace_back(video.sound);

			video.frameCounter++;

			if (!video.loop)                                                    // solamente si el video NO se reproduce en bucle
				if (video.numFrames <= video.frameCounter)                      // controlamos si hemos llegado al final o no
					context->nextVideo();

			video.video->nextFrame();
		}
	}
}

void VideoSystem::fixedUpdate(const Context & context, const float deltaTime) {
	if (context->getComponents().getComponents<Video>().size() > context->getVideoIndex()) {
		auto & video = context->getComponents().getComponents<Video>()[context->getVideoIndex()];

		video.video->render();

		context->getEngine().display();
	}
}
