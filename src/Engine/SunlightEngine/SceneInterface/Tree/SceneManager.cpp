#include <Engine/SunlightEngine/SceneInterface/Tree/SceneManager.hpp>

#include <Engine/SunlightEngine/SceneInterface/Tree/Model.hpp>
#include <Engine/SunlightEngine/SceneInterface/Tree/Animation.hpp>

#include <Engine/SunlightEngine/SunlightEngine.hpp>

#include <Engine/util/glad/glad.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>

SceneManager::SceneManager(SunlightEngine * _engine, ResourceManager * _resourceManager) : engine(_engine), resourceManager(_resourceManager) { }

void SceneManager::render() {
	//if(firstTime)
		//firstTimeRenderConfig();

	lightMatrixConfig();
	renderOffcreen();
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

TreeNode * SceneManager::addAnimatedNode(const std::string_view path) {
	auto tree_ptr = std::make_unique<TreeNode>(*this);
	tree_ptr->setEntity(std::make_unique<Animation>(resourceManager, path));

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

//LIGHT N SHADOW CREATION
TreeNode * SceneManager::addLightNode(const glm::vec3& diff, const glm::vec3& spe, const glm::vec3& dir) {
	auto tree_ptr   = std::make_unique<TreeNode>(*this);
	auto light_ptr  = std::make_unique<Light>(diff, spe);

	if(lights_index < MAX_LIGHT_SIZE)
	{
		lights[lights_index]     = light_ptr.get();
		lightNodes[lights_index] = tree_ptr.get();
		genShadowTexture();
		++lights_index;

		light_ptr->direccion  = dir;
		light_ptr->projection = glm::ortho(-100.f, 100.f, -100.f, 100.f, light_ptr->near, light_ptr->far);
		//light_ptr->projection = glm::perspective(glm::radians(90.f), (float)SHADOW_VP_WIDTH / (float)SHADOW_VP_HEIGHT, light_ptr->near, light_ptr->far);
		tree_ptr->setEntity(std::move(light_ptr));
	}
	else
		std::cout << "Máximo de luces posibles ya creadas\n";

	return root->addChildren(std::move(tree_ptr));
}

void SceneManager::genShadowTexture() {
	//SHADOW TEX
	glGenTextures(1, &lights[lights_index]->shadow_map);
	glBindTexture(GL_TEXTURE_2D, lights[lights_index]->shadow_map);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, SHADOW_VP_WIDTH, SHADOW_VP_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float border_color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color);

	/*
		for(unsigned i=0; i<6; ++i)
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT24, SHADOW_VP_WIDTH, SHADOW_VP_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
		
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
		
			- SE USA SI TRABAJAMOS EN EL SHADER CON SAMPLER_SHADOW_CUBE, SINO COMENTAR LAS 2 LINEAS
			- GL_TEXTURE_COMPARE_MODE -> ESTABLECE QUE FORMA DE COMPARAR INTERNA TENDRÁ. SOLO PARA TEXTURES DEPTH_ATTATCH.
			- GL_LEQUAL  result={1.0  r <= Dt
								{0.0  r  > Dt
				Siendo r  = el valor del fragmento que creamos para comparar.
				Siendo Dt = el valor almacenado en la textura para unas coords dadas.
	*/
	//SHADOW FBO
	glGenFramebuffers(1, &lights[lights_index]->FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, lights[lights_index]->FBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, lights[lights_index]->shadow_map, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)  {
		std::cout << "FBO mal creado\n";
		std::terminate();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void SceneManager::lightMatrixConfig(){

	for(std::size_t i=0; i<lights_index; ++i)
	{
		if(!lights[i] || !lightNodes[i]) continue;

		const vec3& lightPos = lightNodes[i]->getPosition();
		lights[i]->m_VP      = lights[i]->projection * glm::lookAt(lightPos, lightPos + lights[i]->direccion, glm::vec3(0.0, 1.0, 0.0));
	}
}

//MAIN SCENE
void SceneManager::renderScene() {
	camera->setViewMatrix(glm::lookAt(cameraNode->getPosition(), camera->getTarget(), glm::vec3(0, 1, 0)));
	
	view           = camera->getViewMatrix();
	viewProjection = camera->getViewProjectionMatrix();

	shaders[0].enable();
	shaders[0].vec3Uniform("camera_pos", cameraNode->getPosition());
	shaders[0].vec3Uniform("l_Ambient", ambient);

	sendLightsData2ShaderScene();

	root->render(glm::mat4(1), shaders[0], true);

	for(std::size_t i=0; i<lights_index; ++i)
	{
		glActiveTexture(GL_TEXTURE2 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void SceneManager::sendLightsData2ShaderScene() {
	std::string name;

	for(std::size_t i=0; i<lights_index; ++i)
	{
		if(!lights[i] || !lightNodes[i]) continue;

    	std::string name = "lights[" + std::to_string(i) + "]";
		
		glActiveTexture(GL_TEXTURE2 + i);
		glBindTexture(GL_TEXTURE_2D, lights[i]->shadow_map);
		shaders[0].intUniform(name + ".shadow_map", 2 + i);

		shaders[0].mat4Uniform(name + ".m_VP_Light", lights[i]->m_VP);
		shaders[0].vec3Uniform(name + ".position", lightNodes[i]->getPosition());
    	shaders[0].vec3Uniform(name + ".diffuse" , lights[i]->diffuse);
    	shaders[0].vec3Uniform(name + ".specular", lights[i]->specular);
	}
    shaders[0].intUniform("lights_index", lights_index);

}

//OFF-SCREEN 
void SceneManager::renderOffcreen() {
	shaders[1].enable();

	glViewport(0, 0, SHADOW_VP_WIDTH, SHADOW_VP_HEIGHT);
	glCullFace(GL_BACK);

	for(std::size_t i=0; i<lights_index; ++i)
	{
		if(!lights[i] || !lightNodes[i]) continue;
		renderShadow(i);
	}

	glCullFace(GL_FRONT);
	engine->setViewport();
}

void SceneManager::renderShadow(size_t index) {
	glBindFramebuffer(GL_FRAMEBUFFER, lights[index]->FBO);
	glClear(GL_DEPTH_BUFFER_BIT);

	shaders[1].mat4Uniform("m_VP", lights[index]->m_VP);
	
	root->render(glm::mat4(1), shaders[1], false);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void SceneManager::clearScene() {
	root->removeAllChildren();

	cameraNode = nullptr;
	camera = nullptr;

	lights_index = 0;

	ambient = vec3(0.2f);

	view = mat4(1);
	viewProjection = mat4(1);
	lightViewProjection = mat4(1);
}
