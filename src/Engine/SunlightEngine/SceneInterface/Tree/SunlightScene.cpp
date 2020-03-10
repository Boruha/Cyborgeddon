#include <Engine/SunlightEngine/SceneInterface/Tree/SunlightScene.hpp>
#include <Engine/SunlightEngine/SunlightEngine.hpp>
#include <iostream>
#include <glm/gtx/string_cast.hpp>

std::unique_ptr<INode> SunlightScene::addObjectNode(const std::string_view mesh) {
	return std::unique_ptr<INode>();
}

std::unique_ptr<INode> SunlightScene::addCameraNode() {
	return std::unique_ptr<INode>();
}

vec3 SunlightScene::cursorToWorld(const float x, const float y, const float far) {
	const glm::mat4x4& viewProjMatrix = sceneManager->getCamera()->getViewProjectionMatrix();

	// Deshacemos [projection * view] obteniendo su inversa (para pasar de coordenadas del mundo a la pantalla, hay que hacer projection * view)
	const glm::mat4x4& unprojectMatrix = glm::inverse(viewProjMatrix);

	const float VIEWPORT_WIDTH = sceneManager->getEngine()->getViewport().x;
	const float VIEWPORT_HEIGHT  = sceneManager->getEngine()->getViewport().y;

	// Por algun motivo OpenGL lee la Y de la pantalla de abajo a arriba, asi que invertimos la y
	// Hay que normalizar las coordenadas entre -1 (izquierda/abajo) y +1 (derecha/arriba)
	const glm::vec4 viewportPos ((2.f * x / VIEWPORT_WIDTH) - 1.f, (2.f * (VIEWPORT_HEIGHT - y) / VIEWPORT_HEIGHT) - 1.f, 2.f * far - 1.f, 1.f);

	// Obtenemos las coordenadas del mundo en funcion de la distancia "far" calculada en viewportPos (profundidad desde el punto de vista de la camara)
	const glm::vec4 worldPos(unprojectMatrix * viewportPos);

	return worldPos.w == 0 ? vec3() : vec3(worldPos.x, worldPos.y, worldPos.z) / worldPos.w;
}

INode * SunlightScene::addMeshNode(const std::string_view mesh) {
	return sceneManager->addMeshNode(mesh);
}

INode * SunlightScene::addFreeCameraNode() {
	return sceneManager->addCameraNode();
}
