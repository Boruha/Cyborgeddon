#include <Engine/IrrlichtEngine/SceneInterface/IrrlichtScene.hpp>
#include <Engine/IrrlichtEngine/SceneInterface/IrrlichtObjectNode.hpp>
#include <Engine/IrrlichtEngine/SceneInterface/IrrlichtCameraNode.hpp>

#include <irrlicht/ISceneManager.h>
#include <irrlicht/IVideoDriver.h>

std::unique_ptr<INode> IrrlichtScene::addObjectNode(const vec3 * pos, const vec3 * rot, const vec3 * sca) {
    return std::make_unique<IrrlichtObjectNode>(sceneManager, pos, rot, sca);
}

std::unique_ptr<INode> IrrlichtScene::addCameraNode(const vec3 * pos, const vec3 * rot, const vec3 * sca, const vec3 * tar) {
    auto u_camera = std::make_unique<IrrlichtCameraNode>(sceneManager, pos, rot, sca, tar);
    camera = u_camera.get();
	return std::move(u_camera);
}

vec3 IrrlichtScene::cursorToWorld(const float x, const float y, const float far) const {
	const glm::mat4x4& projectionMatrix = camera->getProjectionMatrix();
	const glm::mat4x4& viewMatrix = camera->getViewMatrix();

	// Deshacemos [projection * view] obteniendo su inversa (para pasar de coordenadas del mundo a la pantalla, hay que hacer projection * view)
	const glm::mat4x4& unprojectMatrix = glm::inverse(projectionMatrix * viewMatrix);

	const float VIEWPORT_HEIGHT = sceneManager->getVideoDriver()->getViewPort().getHeight();
	const float VIEWPORT_WIDTH  = sceneManager->getVideoDriver()->getViewPort().getWidth();

	// Por algun motivo OpenGL lee la Y de la pantalla de abajo a arriba, asi que invertimos la y
	// Hay que normalizar las coordenadas entre -1 (izquierda/abajo) y +1 (derecha/arriba)
	const glm::vec4 viewportPos ((2.0 * x / VIEWPORT_WIDTH) - 1, (2.0 * (VIEWPORT_HEIGHT - y) / VIEWPORT_HEIGHT) - 1.0, 2.0 * far - 1.0, 1.0);

	// Obtenemos las coordenadas del mundo en funcion de la distancia "far" calculada en viewportPos (profundidad desde el punto de vista de la camara)
	const glm::vec4 worldPos(unprojectMatrix * viewportPos);

	return worldPos.w == 0 ? vec3() : vec3(worldPos.x, worldPos.y, worldPos.z) / worldPos.w;
}

void IrrlichtScene::loadTexture(const std::string& path) const {
	sceneManager->getVideoDriver()->getTexture(path.c_str());
}

void IrrlichtScene::unloadTexture(const std::string& path) const {
	sceneManager->getVideoDriver()->removeTexture(sceneManager->getVideoDriver()->getTexture(path.c_str()));
}

void IrrlichtScene::unloadTextures() const {
	sceneManager->getVideoDriver()->removeAllTextures();
}