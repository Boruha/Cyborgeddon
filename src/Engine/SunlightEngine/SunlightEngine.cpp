#include <Engine/SunlightEngine/SunlightEngine.hpp>

extern "C" {
	#include <Engine/util/glad/glad.h>
	#include <GLFW/glfw3.h>
}

#include <Engine/SunlightEngine/SceneInterface/Tree/SunlightScene.hpp>
#include <Engine/util/Color.hpp>
#include <Engine/util/Error.hpp>

void framebuffer_size_callback(GLFWwindow * const window, const int width, const int height) {
	glViewport(0, 0, width, height);
}

void SunlightEngine::init(const unsigned width, const unsigned height, const std::string_view name) {
	
	// permitimos editar glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	windowWidth = width;
	windowHeight = height;

	window = glfwCreateWindow(int(width), int(height), name.data(), nullptr, nullptr);
	
	if (!window)
		error("No se pudo abrir la ventana");

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
   		std::cout << "Failed to initialize GLAD\n";
    	std::terminate();
	} 

	glEnable(GL_DEPTH_TEST);

	resourceManager = std::make_unique<ResourceManager>();
	sceneManager    = std::make_unique<SceneManager>(this, resourceManager.get());
	scene           = std::make_unique<SunlightScene>(sceneManager.get());
}

bool SunlightEngine::run() const {
	glfwPollEvents();

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void SunlightEngine::draw() const {
	sceneManager->render();
}

void SunlightEngine::display() const {
	glfwSwapBuffers(window);
}

void SunlightEngine::setViewport(){
	glfwGetWindowSize(window, (int*)&windowWidth, (int*)&windowHeight);
	glViewport(0, 0, windowWidth, windowHeight);

}