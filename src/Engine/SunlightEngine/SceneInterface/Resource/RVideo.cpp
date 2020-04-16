#include <Engine/SunlightEngine/SceneInterface/Resource/RVideo.hpp>

//#include <GL/glew.h>
//#include <GLFW/glfw3.h>


extern "C" {
	#include <Engine/util/glad/glad.h>
	#include <GLFW/glfw3.h>
}

#include <Engine/util/Error.hpp>

RVideo::RVideo(const std::string_view path) {
	capture.open(path.data());

	if (!capture.isOpened())
		error("No se pudo abrir el video " + std::string(path));

	frame.reserve(capture.get(CV_CAP_PROP_FRAME_COUNT));

	glGenFramebuffers(1, &FBO);

	if (!capture.read(frame))
		error("El video esta vacio " + std::string(path));
}

RVideo::~RVideo() {
	glDeleteFramebuffers(1, &FBO);
}

void RVideo::render() const {
	glEnable(GL_TEXTURE_2D);

	unsigned texture = getTextureFromFrame();

	glBindFramebuffer(GL_READ_FRAMEBUFFER, FBO);

	glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

	int width, height;

	glfwGetWindowSize(glfwGetCurrentContext(), &width, &height);

	glBlitFramebuffer(0, 0, frame.cols, frame.rows, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_LINEAR);

	glDeleteTextures(1, &texture);

	glDisable(GL_TEXTURE_2D);
}

unsigned RVideo::getTextureFromFrame() const {
	unsigned textureID {};
	glGenTextures(1, &textureID);

	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frame.cols, frame.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, frame.ptr());

	return textureID;
}

void RVideo::reset() {
	capture.set(CV_CAP_PROP_POS_FRAMES, 0);
}

void RVideo::nextFrame() {
	std::cout << capture.get(CV_CAP_PROP_POS_FRAMES) << "\n";

	if (capture.get(CV_CAP_PROP_POS_FRAMES) == capture.get(CV_CAP_PROP_FRAME_COUNT)) {
		if (loopVideo)
			reset();
	}

	capture.read(frame);
}
