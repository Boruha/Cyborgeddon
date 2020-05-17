#include <cmp/Video.hpp>

#include <ostream>

Video::Video(const EntityType e_type, const EntityID e_ID, const std::string_view path) : Component(e_type, e_ID) {
	const size_t pos = path.find_last_of('/');

	if (pos != std::string::npos)
		name = std::string(path.begin() + pos + 1, path.end());
	else
		name = path;
}

std::ostream & operator<<(std::ostream & os, const Video & vid) {
	vid.print(os, Video::getName())
	<< "\n\tIVideo: "                       << vid.video
	<< "\n\tName: "                         << vid.name
	<< "\n\tSound: "                        << vid.sound
	<< "\n\tFrames: "                       << vid.numFrames
	<< "\n\tFrame actual: "                 << vid.frameCounter
	<< "\n\tTiempo por frame: "             << vid.timePerFrame
	<< "\n\tTiempo desde el ultimo frame: " << vid.timeSinceLastFrame
	<< "\n\tBucle: " << std::boolalpha      << vid.loop;

	return os;
}
