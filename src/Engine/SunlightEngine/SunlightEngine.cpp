#include <Engine/SunlightEngine/SunlightEngine.hpp>

#include <Engine/util/Color.hpp>

extern "C" {
	#include <Engine/util/glad/glad.h>
	#include <GLFW/glfw3.h>
}

#include <iostream>
#include <codecvt>
#include <locale>

void error(const std::string & message) {
	std::cerr << message << std::endl;
	glfwTerminate();
	exit(-1);
}

void framebuffer_size_callback(GLFWwindow * const window, const int width, const int height) {
	glViewport(0, 0, width, height);
}

void SunlightEngine::init(const unsigned width, const unsigned height, const std::wstring& name) {
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	windowWidth = width;
	windowHeight = height;

	window = glfwCreateWindow(int(width), int(height), std::string(name.begin(), name.end()).c_str(), nullptr, nullptr);

	if (!window)
		error("No se pudo abrir la ventana");

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
		error("No se pudo inicializar glad");

	glEnable(GL_DEPTH_TEST);
}

bool SunlightEngine::run() const {
	return !glfwWindowShouldClose(window);
}

void SunlightEngine::shutdown() const {
	glfwTerminate();
}

bool SunlightEngine::isKeyPressed(const KEY_CODE code) const {
	return glfwGetKey(window, code) == GLFW_PRESS;
}

const Mouse & SunlightEngine::getMouse() {
	if (glfwGetWindowAttrib(window, GLFW_FOCUSED)) {
		double x, y;
		glfwGetCursorPos(window, &x,  &y);

		if (x >= 0 && x < windowWidth && y >= 0 && y < windowHeight) { // si el cursor esta dentro de la ventana
			mouse.position = glm::vec2(x, y);
			mouse.leftPressed = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
			mouse.rightPressed = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;
		}
	}

	return mouse;
}

void SunlightEngine::clear(const Color color) const {
	glClearColor(float(color.r) / 255.f, float(color.g) / 255.f, float(color.b) / 255.f, float(color.a) / 255.f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void SunlightEngine::draw() const {

}

void SunlightEngine::display() const {
	glfwSwapBuffers(window);
	glfwPollEvents();
}