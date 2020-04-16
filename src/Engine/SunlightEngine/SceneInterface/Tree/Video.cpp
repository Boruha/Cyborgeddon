#include <Engine/SunlightEngine/SceneInterface/Tree/Video.hpp>

#include <Engine/SunlightEngine/SceneInterface/Resource/ResourceManager.hpp>

Video::Video(ResourceManager * const resManager, const std::string_view path) : resourceManager(*resManager) {
	if (!path.empty())
		setVideo(path);
}

void Video::setVideo(const std::string_view path) {
	video = &resourceManager.getVideo(path);
}

void Video::render() const {
	video->render();
}

void Video::reset() {
	video->reset();
}

void Video::nextFrame() {
	video->nextFrame();
}

double Video::getNumFrames() {
	return video->getNumFrames();
}

double Video::getFPS() {
	return video->getFPS();
}

void Video::setLoop(const bool loop) {
	video->setLoop(loop);
}
