#include <Engine/SunlightEngine/SunlightEngine.hpp>

#include <Engine/util/Color.hpp>

extern "C" {
	#include <Engine/util/glad/glad.h>
	#include <GLFW/glfw3.h>
}

#include <Engine/util/Error.hpp>
#include <Engine/SunlightEngine/SceneInterface/Tree/SunlightScene.hpp>

void framebuffer_size_callback(GLFWwindow * const window, const int width, const int height) {
	glViewport(0, 0, width, height);
}

void SunlightEngine::init(const unsigned width, const unsigned height, const std::string_view name) {
	// permitimos editar glfw
	glfwInit();

	// le decimos la version de opengl que debe soportar
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);

	// como ya hemos establecido la version de opengl que debe soportar,
	// al usar core profile, no va a controlar si lo que hagamos lo va a
	// soportar o no porque nosotros le decimos explicitamente que es lo
	// que queremos que soporte al establecer la version
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	windowWidth = width;
	windowHeight = height;

	window = glfwCreateWindow(int(width), int(height), name.data(), nullptr, nullptr);

	if (!window)
		error("No se pudo abrir la ventana");

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// usaremos glad para asegurarnos de que cualquier llamada que hagamos
	// a opengl es la especifica de nuestro sistema operativo
	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
		error("No se pudo inicializar glad");

	// opengl no dibujara lo que este detras de algo ya dibujado
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);

	resourceManager = std::make_unique<ResourceManager>();
	sceneManager = std::make_unique<SceneManager>(this, resourceManager.get());

	scene = std::make_unique<SunlightScene>(sceneManager.get());
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
	glClear(unsigned(GL_COLOR_BUFFER_BIT) | unsigned(GL_DEPTH_BUFFER_BIT));
}

void SunlightEngine::draw() const {
	sceneManager->render();
}

void SunlightEngine::display() const {
	glfwSwapBuffers(window);
}