#include <sys/VideoSystem.hpp>
#include <Engine/EngineInterface/SceneInterface/IVideo.hpp>

void VideoSystem::update(const Context &context, const float deltaTime) {
	if (context->getComponents().getComponents<Video>().size() > context->getVideoIndex()) {

		auto& video = context->getComponents().getComponents<Video>()[context->getVideoIndex()];

		if ((video.timeSinceLastFrame += deltaTime) >= video.timePerFrame) {    // primero controlamos si debemos cambiar de frame
			video.timeSinceLastFrame -= video.timePerFrame;                     // si es asi, actualizamos el contador de tiempo

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
