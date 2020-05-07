#include <Engine/SunlightEngine/SceneInterface/Resource/RTexture.hpp>

#include <Engine/util/glad/glad.h>
#include <GLFW/glfw3.h>

#include <Engine/util/Error.hpp>

extern "C" {
	#ifndef STB_IMAGE_IMPLEMENTATION
	#define STB_IMAGE_IMPLEMENTATION
	#include <Engine/util/stb_image/stb_image.h>
	#endif
}

RTexture::RTexture(const std::string_view path) {
	int formatColor;

//	stbi_set_flip_vertically_on_load(true);

	unsigned char * textureData = stbi_load(path.data(), &width, &height, &formatColor, STBI_rgb_alpha);

	if (!textureData)
		error("No se pudo cargar la textura " + std::string(path));

	glGenTextures(1, &texture.ID);

	GLenum colorFormat { };

	switch (formatColor) {
		case 1  : colorFormat = GL_RED;  break;
		case 3  : colorFormat = GL_RGB;  break;
		case 4  : colorFormat = GL_RGBA; break;
		default : error("Formato de color no soportado en la textura " + std::string(path)); break;
	}

	glBindTexture(GL_TEXTURE_2D, texture.ID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, colorFormat, GL_UNSIGNED_BYTE, textureData);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_image_free(textureData);

	glGenVertexArrays(1, &VAO);

	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	setupTexture();

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<const void *>(0 * sizeof(float)));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<const void *>(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	shader.enable();
	shader.intUniform("uTexture", 0);
}

RTexture::~RTexture() {
	glDeleteTextures(1, &texture.ID);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void RTexture::setPosition(const unsigned int _x, const unsigned int _y) {
	x = _x;
	y = _y;

	glBindVertexArray(VAO);
	setupTexture();
	glBindVertexArray(0);
}

void RTexture::setupTexture() const {
	const GLFWvidmode * videomode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	const float glX = 2.f * float(x) / videomode->width  - 1;
	const float glY = 2.f * float(y) / videomode->height - 1;

	const float glWidth  = 2 * float(x + width)  / videomode->width  - 1;
	const float glHeight = 2 * float(y + height) / videomode->height - 1;

	const float vertices[] {
			glWidth,      -glY, 0.0f,   1.0f, 0.0f,  // top right
			glWidth, -glHeight, 0.0f,   1.0f, 1.0f,  // bottom right
			glX,     -glHeight, 0.0f,   0.0f, 1.0f, // bottom left
			glWidth,      -glY, 0.0f,   1.0f, 0.0f,  // top right
			glX,     -glHeight, 0.0f,   0.0f, 1.0f, // bottom left
			glX,          -glY, 0.0f,   0.0f, 0.0f// top left
	};

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

void RTexture::setSize(const unsigned int w, const unsigned int h) {
	width = w;
	height = h;

	glBindVertexArray(VAO);
	setupTexture();
	glBindVertexArray(0);
}

void RTexture::setWidth(const unsigned int w) {
	width = w;

	glBindVertexArray(VAO);
	setupTexture();
	glBindVertexArray(0);
}

void RTexture::setHeight(const unsigned int h) {
	height = h;

	glBindVertexArray(VAO);
	setupTexture();
	glBindVertexArray(0);
}

void RTexture::render() const {
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture.ID);

	glUseProgram(shader.getID());

	glBindVertexArray(VAO);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(0);

	glDisable( GL_BLEND );
}
