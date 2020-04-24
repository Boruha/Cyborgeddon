#include <Engine/SunlightEngine/SceneInterface/Tree/SceneManager.hpp>

#include <Engine/SunlightEngine/SceneInterface/Tree/Model.hpp>
#include <Engine/SunlightEngine/SunlightEngine.hpp>

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <GL/glew.h>

SceneManager::SceneManager(SunlightEngine * _engine, ResourceManager * _resourceManager) : engine(_engine), resourceManager(_resourceManager) {
	//SHADOW FBO
	glGenFramebuffers(1, &shadowFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);

	//SHADOW TEXTURE
	glGenTextures(1, &shadowSceneTexture);
	glBindTexture(GL_TEXTURE_2D, shadowSceneTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_VP_WIDTH, SHADOW_VP_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float border_color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D , shadowSceneTexture, 0);
	
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ER BUFFER EHPLOTÓ\n";
		exit(-1);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void SceneManager::render() {
	renderShadows();
	renderScene();
}

glm::mat4 SceneManager::getLightViewProjection() const {
	return lightViewProjection;
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

	if(lights_index < MAX_LIGHT_SIZE)
	{
		lights[lights_index]     = light_ptr.get();
		lightNodes[lights_index] = tree_ptr.get();
		++lights_index;

		light_ptr->projection = glm::ortho<float>(-100, 100, -100, 100, 0.1, 300);

		tree_ptr->setEntity(std::move(light_ptr));
	}
	else
		std::cout << "Alv esa luz\n";

	return root->addChildren(std::move(tree_ptr));
}

void SceneManager::setLights() {
	std::string name;

	for(std::size_t i=0; i<lights_index; ++i)
	{
		if(!lights[i] || !lightNodes[i]) continue;

    	std::string name = "lights[" + std::to_string(lights[i]->getID()) + "]";

		shaders[0].vec3Uniform(name + ".position", lightNodes[i]->getPosition());
    	shaders[0].vec3Uniform(name + ".ambient" , lights[i]->getAmbient());
    	shaders[0].vec3Uniform(name + ".diffuse" , lights[i]->getDiffuse());
    	shaders[0].vec3Uniform(name + ".specular", lights[i]->getSpecular());
	}
    shaders[0].intUniform("light_index", lights_index);

}

void SceneManager::renderScene() {
	camera->setViewMatrix(glm::lookAt(cameraNode->getPosition(), camera->getTarget(), glm::vec3(0, 1, 0)));
	
	view           = camera->getViewMatrix();
	viewProjection = camera->getViewProjectionMatrix();

	shaders[0].enable();
	shaders[0].vec3Uniform("camera_pos", cameraNode->getPosition());
	shaders[0].mat4Uniform("m_LightVP", lightViewProjection);

	setLights();

	glActiveTexture(GL_TEXTURE2);
	shaders[0].intUniform("shadow_map", 2);
	glBindTexture(GL_TEXTURE_2D, shadowSceneTexture);

	root->render(glm::mat4(1), shaders[0], true);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void SceneManager::setShadows(){
	lights[0]->view = glm::lookAt(glm::vec3(lightNodes[0]->getPosition()),
								glm::vec3(0.f),
								glm::vec3(0.f, 1.f, 0.f));

	lightViewProjection = lights[0]->projection * lights[0]->view;
}

void SceneManager::renderShadows() {

	shaders[1].enable();
	
	glViewport(0, 0, SHADOW_VP_WIDTH, SHADOW_VP_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	glCullFace(GL_BACK);

	setShadows();
	root->render(glm::mat4(1), shaders[1], false);

	glCullFace(GL_FRONT);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, 1280, 720);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}