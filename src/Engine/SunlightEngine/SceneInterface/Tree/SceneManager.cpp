#include <glm/ext/matrix_transform.hpp>

#include <Engine/SunlightEngine/SceneInterface/Tree/SceneManager.hpp>
#include <Engine/SunlightEngine/SceneInterface/Tree/Model.hpp>
#include <iostream>
#include <glm/gtx/string_cast.hpp>

void SceneManager::render() {
	camera->setViewMatrix(glm::lookAt(cameraNode->getPosition(), camera->getTarget(), glm::vec3(0, 1, 0)));
	view = camera->getViewMatrix();
	viewProjection = camera->getViewProjectionMatrix();
	
	shader.enable();
	shader.vec3Uniform("light.position", lightNode->getPosition());

	root->render(glm::mat4(1), shader);
}

glm::mat4 SceneManager::getViewProjection() const {
	return viewProjection;
}

glm::mat4 SceneManager::getView() const {
	return view;
}

TreeNode * SceneManager::addMeshNode(const std::string_view mesh) {
	auto tree_ptr = std::make_unique<TreeNode>(*this);
	tree_ptr->setEntity(std::make_unique<Model>(resourceManager, mesh));

	return root->addChildren(std::move(tree_ptr));
}

TreeNode * SceneManager::addCameraNode() {
	auto tree_ptr   = std::make_unique<TreeNode>(*this);
	auto camera_ptr = std::make_unique<Camera>();

	camera = camera_ptr.get();
	camera->setPerspective(0.1f, 1000.f);

	tree_ptr->setEntity(std::move(camera_ptr));

	cameraNode = tree_ptr.get();

	return root->addChildren(std::move(tree_ptr));
}

TreeNode * SceneManager::addLightNode(const glm::vec3& amb, const glm::vec3& diff, const glm::vec3& spe) {
	auto tree_ptr   = std::make_unique<TreeNode>(*this);
	auto light_ptr  = std::make_unique<Light>(amb, diff, spe);

	light = light_ptr.get();
	tree_ptr->setEntity(std::move(light_ptr));
	lightNode = tree_ptr.get();

	return root->addChildren(std::move(tree_ptr));
}
