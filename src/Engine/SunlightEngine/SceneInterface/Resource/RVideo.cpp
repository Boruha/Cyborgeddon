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

	if (!capture.read(frame))
		error("El video esta vacio " + std::string(path));

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	const float vertices[] {
			 1.0f,  1.0f, 0.0f,   1.0f, 1.0f, // top right
			 1.0f, -1.0f, 0.0f,   1.0f, 0.0f, // bottom right
			-1.0f, -1.0f, 0.0f,   0.0f, 0.0f, // bottom left
			 1.0f,  1.0f, 0.0f,   1.0f, 1.0f, // top right
			-1.0f, -1.0f, 0.0f,   0.0f, 0.0f, // bottom left
			-1.0f,  1.0f, 0.0f,   0.0f, 1.0f  // top left
	};


	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<const void *>(0 * sizeof(float)));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<const void *>(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	shader.enable();
	shader.intUniform("uTexture", 0);
}

RVideo::~RVideo() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void RVideo::render() const {
/*
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	unsigned texture = getTextureFromFrame();

	glBindFramebuffer(GL_READ_FRAMEBUFFER, FBO);
	glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
	int width, height;

	glfwGetWindowSize(glfwGetCurrentContext(), &width, &height);

	glBlitFramebuffer(0, 0, frame.cols, frame.rows, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_LINEAR);
	glDeleteTextures(1, &texture);
	glDisable( GL_BLEND );
*/

	unsigned texture = getTextureFromFrame();

	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glUseProgram(shader.getID());

	glBindVertexArray(VAO);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(0);

	glDisable( GL_BLEND );

	glDeleteTextures(1, &texture);
}

unsigned RVideo::getTextureFromFrame() const {
	unsigned textureID {};
	glGenTextures(1, &textureID);

	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, frame.cols, frame.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, frame.ptr());

	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return textureID;
}

void RVideo::reset() {
	capture.set(CV_CAP_PROP_POS_FRAMES, 0);
}

void RVideo::nextFrame() {
	if (capture.get(CV_CAP_PROP_POS_FRAMES) == capture.get(CV_CAP_PROP_FRAME_COUNT)) {
		if (loopVideo)
			reset();
	}

	capture.read(frame);
}
