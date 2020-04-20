#include <Engine/SunlightEngine/SceneInterface/Tree/SceneManager.hpp>

#include <Engine/SunlightEngine/SceneInterface/Tree/Model.hpp>
#include <Engine/SunlightEngine/SunlightEngine.hpp>

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>

SceneManager::SceneManager(SunlightEngine * _engine, ResourceManager * _resourceManager) : engine(_engine), resourceManager(_resourceManager) {
	//pasamos al context segundo plano para setar la config de las sombras
	engine->setContext(false);

	//creation and config the shadow map
	glGenTextures(1, &shadowSceneTexture);
	glBindTexture(GL_TEXTURE_2D, shadowSceneTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_VP_WIDTH, SHADOW_VP_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	GLfloat color[] = { 1.f, 1.f, 1.f, 1.f };
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, *color);

	//Creation and config of framebuffer that will work w/shadows
	glGenBuffers(1, &shadowFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowSceneTexture, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	if(!(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE))
	{
		std::cout << "ER BUFFER EHPLOTÓ\n";
		exit(-1);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	//volvemos context principal
	engine->setContext(true);
}

void SceneManager::render() {
	renderShadows();
	renderScene();
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

		light_ptr->projection = glm::ortho(-10.f, 10.f, -10.f, 10.f, 0.1f, 1000.f);

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
	setLights();

	root->render(glm::mat4(1), shaders[0]);
}

void SceneManager::setShadows(){
	std::string name;

	for(std::size_t i=0; i<lights_index; ++i)
	{
		if(!lights[i] || !lightNodes[i]) continue;

		lights[i]->view = glm::lookAt(glm::vec3(lightNodes[i]->getPosition()),
									  glm::vec3(0.f),
									  glm::vec3(0.f, 1.f, 0.f));

    	std::string name = "lights[" + std::to_string(lights[i]->getID()) + "]";

		//shaders[0].vec3Uniform(name + ".position", lightNodes[i]->getPosition());
    	//shaders[0].vec3Uniform(name + ".ambient" , lights[i]->getAmbient());
    	//shaders[0].vec3Uniform(name + ".diffuse" , lights[i]->getDiffuse());
    	//shaders[0].vec3Uniform(name + ".specular", lights[i]->getSpecular());
	}
    //shaders[1].intUniform("light_index", lights_index);
}

void SceneManager::renderShadows() {
	engine->setContext(false);

	glBindBuffer(GL_FRAMEBUFFER, shadowFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	//setLightMatrices();
	//root->render(glm::mat4(1), shaders[1]);
	glBindBuffer(GL_FRAMEBUFFER, 0);

	engine->setContext(true);
}