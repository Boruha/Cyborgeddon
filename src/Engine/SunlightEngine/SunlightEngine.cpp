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
	auto * sunlightEngine = static_cast<SunlightEngine*>(glfwGetWindowUserPointer(window));

	sunlightEngine->setViewport( { width, height } );
}

void key_callback(GLFWwindow* window, const int key, const int scancode, const int action, const int mods)
{
	auto * sunlightEngine = static_cast<SunlightEngine*>(glfwGetWindowUserPointer(window));

	if (action == GLFW_PRESS) {
		if (key >= 97 && key <= 122)
			sunlightEngine->setTextKeyInput(key - 32);
		else
			sunlightEngine->setTextKeyInput(key);
	}
}

void SunlightEngine::init(const unsigned width, const unsigned height, const std::string_view name) {
	
	// permitimos editar glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	setViewport( { width, height } );

	GLFWmonitor * monitor = glfwGetPrimaryMonitor();

	if (!monitor)
		error("No se detectó el monitor");

	const GLFWvidmode * videoMode = glfwGetVideoMode(monitor);

	if (!videoMode)
		error("No se pudo identificar el modo de video");
	else
		setViewport( { videoMode->width, videoMode->height } );

    window = glfwCreateWindow(int(windowWidth), int(windowHeight), name.data(), monitor, nullptr);

	if (!window)
		error("No se pudo abrir la ventana");

	glfwMakeContextCurrent(window);
	glfwSetWindowUserPointer(window, this);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);

	// usaremos glad para asegurarnos de que cualquier llamada que hagamos
	// a opengl es la especifica de nuestro sistema operativo
	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
		error("No se pudo inicializar glad");


	glEnable(GL_DEPTH_TEST);

	resourceManager = std::make_unique<ResourceManager>();
	sceneManager    = std::make_unique<SceneManager>(this, resourceManager.get());
	scene           = std::make_unique<SunlightScene>(sceneManager.get());
	textKeyInput    = GLFW_KEY_UNKNOWN;
}

bool SunlightEngine::run() {

	textKeyInput = GLFW_KEY_UNKNOWN;

	glfwPollEvents();

	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
		close();

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

bool SunlightEngine::isKeyPressed(const unsigned code) const {
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

void SunlightEngine::clear(const Color color, const bool depth) const {
	glClearColor(float(color.r) / 255.f, float(color.g) / 255.f, float(color.b) / 255.f, float(color.a) / 255.f);

	glClear(unsigned(GL_COLOR_BUFFER_BIT));

	if (depth)
		glClear(unsigned(GL_DEPTH_BUFFER_BIT));
}

void SunlightEngine::draw() const {
	sceneManager->render();
}

void SunlightEngine::display() const {
	glfwSwapBuffers(window);
}

bool SunlightEngine::isKeyTextInput(const unsigned int code) {
	return textKeyInput == code;
}

void SunlightEngine::setViewport(){
	glfwGetWindowSize(window, (int*)&windowWidth, (int*)&windowHeight);
	glViewport(0, 0, windowWidth, windowHeight);
}

void SunlightEngine::close() const {
	glfwSetWindowShouldClose(window, true);
}
