#include <sys/VideoSystem.hpp>
#include <Engine/EngineInterface/SceneInterface/IVideo.hpp>

void VideoSystem::update(const Context &context, const float deltaTime) {
	auto & video = context->getComponents().getComponents<Video>()[context->getVideoIndex()];

	if ((video.timeSinceLastFrame += deltaTime) >= video.timePerFrame) {    // primero controlamos si debemos cambiar de frame
		video.timeSinceLastFrame -= video.timePerFrame;                     // si es asi, actualizamos el contador de tiempo

		if (!video.loop) {                                                  // solamente si el video NO se reproduce en bucle
			if (video.numFrames <= ++video.frameCounter) {                  // controlamos si hemos llegado al final o no
				context->nextVideo();
			}
		}

//		video.video->render();
		video.video->nextFrame();
	}
}

void VideoSystem::fixedUpdate(const Context &context, float deltaTime) {
	auto & video = context->getComponents().getComponents<Video>()[context->getVideoIndex()];

	video.video->render();

	context->getEngine().display();
}
