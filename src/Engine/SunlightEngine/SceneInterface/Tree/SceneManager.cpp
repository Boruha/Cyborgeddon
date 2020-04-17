#include <glm/ext/matrix_transform.hpp>

#include <Engine/SunlightEngine/SceneInterface/Tree/SceneManager.hpp>
#include <Engine/SunlightEngine/SceneInterface/Tree/Model.hpp>
#include <iostream>
#include <glm/gtx/string_cast.hpp>

void SceneManager::render() {
	camera->setViewMatrix(glm::lookAt(cameraNode->getPosition(), camera->getTarget(), glm::vec3(0, 1, 0)));
	view = camera->getViewMatrix();
	viewProjection = camera->getViewProjectionMatrix();
	shader.vec3Uniform("camera_pos", cameraNode->getPosition());
	shader.enable();

	setLights();

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

	if(lights_index < max_light_size)
	{
		lights[lights_index]     = light_ptr.get();
		lightNodes[lights_index] = tree_ptr.get();
		++lights_index;

		tree_ptr->setEntity(std::move(light_ptr));
	} 
	else
		std::cout << "Alv esa luz\n";

	return root->addChildren(std::move(tree_ptr));
}

void SceneManager::setLights()
{
	std::string name;

	for(std::size_t i=0; i<lights_index; ++i)
	{
		if(!lights[i] || !lightNodes[i]) continue;

    	std::string name = "lights[" + std::to_string(lights[i]->getID()) + "]";

		shader.vec3Uniform(name + ".position", lightNodes[i]->getPosition());
    	shader.vec3Uniform(name + ".ambient" , lights[i]->getAmbient());
    	shader.vec3Uniform(name + ".diffuse" , lights[i]->getDiffuse());
    	shader.vec3Uniform(name + ".specular", lights[i]->getSpecular());
	}
    shader.intUniform("light_index", lights_index);

}