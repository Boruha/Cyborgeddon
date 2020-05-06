#include <sys/HUDintroSystem.hpp>

#include <Engine/EngineInterface/SceneInterface/ITexture.hpp>
#include <Engine/EngineInterface/SceneInterface/IVideo.hpp>


void HUDintroSystem::fixedUpdate(const Context &context, const float deltaTime) {
	context->getEngine().clear(Color(BLACK), true);

	if (context->getVideoIndex() > 0)
		for (const auto & texture : context->getComponents().getComponents<TextureCmp>())
			texture.texture->render();
/*
	auto & video = context->getComponents().getComponents<Video>()[context->getVideoIndex()];

	if (!video.loop) {                                                  // solamente si el video NO se reproduce en bucle
		if (video.numFrames <= ++video.frameCounter) {                  // controlamos si hemos llegado al final o no
			context->nextVideo();
		}
	}

	video.video->render();
	video.video->nextFrame();

	context->getEngine().display();
 */
}
