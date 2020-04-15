#include <Engine/SunlightEngine/SceneInterface/Resource/RVideo.hpp>

RVideo::RVideo(const std::string_view path) {
	capture.open(path.data());

	if (!capture.isOpened())
		error("No se pudo abrir el video " + std::string(path));

	frame.reserve(capture.get(CV_CAP_PROP_FRAME_COUNT));

	glGenFramebuffers(1, &FBO);
}

RVideo::~RVideo() {
	glDeleteFramebuffers(1, &FBO);
	frame.release();
	capture.release();
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

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	GLenum format = GL_BGR;

	if (frame.channels() == 1)
		format = GL_LUMINANCE;

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frame.cols, frame.rows, 0, format, GL_UNSIGNED_BYTE, frame.ptr());

	return textureID;
}
