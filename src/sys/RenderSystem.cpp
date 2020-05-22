#include <sys/RenderSystem.hpp>

#include <glm/gtx/string_cast.hpp>
#include <util/MeshPaths.hpp>

void RenderSystem::init(){
	animationMessages.reserve(40);
}

void RenderSystem::reset(){
	animationMessages.clear();
	animationMessages.reserve(16);
}

void RenderSystem::fixedUpdate(const Context& context, float deltaTime) {
	const auto * playerPhysics = context->getPlayer().getComponent<Physics>();
	const auto & cameraNode    = context->getCamera().getComponent<Render>()->node;

	if (playerPhysics) 
	{
		cameraNode->setTarget(playerPhysics->position);

		for (auto & cmp : context->getComponents().getComponents<Render>()) 
		{
			if (cmp && cmp.needUpdate) 
			{
				cmp.node->setPosition(*cmp.pos);
				cmp.node->setRotation(*cmp.rot);
			}
		}
	}

	animationTrigger(context);

	const auto& iengine = context->getEngine();

	iengine.clear(Color(BLACK), true);
	iengine.draw();
}

void RenderSystem::animationTrigger(const Context& context) {
	auto& storage = context->getComponents();

	for(auto& message : animationMessages)
	{
		auto& entity        = context->getEntityByID(message.ID);
		auto* renderCmp_ptr = entity.getComponent<Render>();
		auto* node_ptr      = renderCmp_ptr->node;

		if(renderCmp_ptr->aniMode == message.animationMode) continue;
		   renderCmp_ptr->aniMode  = message.animationMode;

		switch (entity.getType())
		{
			case PLAYER : 	pjAnimationSwitch(storage, node_ptr, message.animationMode);
							break;

			case ENEMY  :
						{	auto mode = entity.getComponent<CharacterData>()->mode;
							
							switch (mode)
							{
								case NEUTRAL: botAnimationSwitch(storage, node_ptr, message.animationMode);
										break;

								case DEMON  : demonAnimationSwitch(storage, node_ptr, message.animationMode);
										break;

								case ANGEL  : angelAnimationSwitch(storage, node_ptr, message.animationMode);
										break;
								default     : break;
							}
						
							break;
						}

			case DOOR 	: /* do smth */
					  		break;	

			default   :		break;
		}
	}

	animationMessages.clear();
}

void RenderSystem::pjAnimationSwitch(Storage& storage, INode * node_ptr, const unsigned aniMode){
	switch (aniMode)
	{
		case 0: storage.setMesh(node_ptr, MESH_DEX_STATIC);
		break;

		case 1: storage.setAnimatedMesh(node_ptr, ANIMATED_DEX_WALKING);
		break;

		case 2: storage.setAnimatedMesh(node_ptr, ANIMATED_DEX_WALKING_ATTACK);
		break;

		case 4: storage.setMesh(node_ptr, MESH_DEX_DASHING);
		break;

		case 5: storage.setAnimatedMesh(node_ptr, ANIMATED_DEX_STATIC_ATTACK);
		break;
	}
}

void RenderSystem::botAnimationSwitch(Storage& storage, INode* node_ptr, const unsigned aniMode){
	switch (aniMode)
	{
		case 0: storage.setMesh(node_ptr, MESH_BOT_STATIC);
		break;

		case 1: storage.setAnimatedMesh(node_ptr, ANIMATED_BOT_WALKING);
		break;

		case 2: storage.setAnimatedMesh(node_ptr, ANIMATED_BOT_ATTACK);
		break;
	}
}

void RenderSystem::demonAnimationSwitch(Storage& storage, INode* node_ptr, const unsigned aniMode){ 
	switch (aniMode)
	{
		case 0: storage.setMesh(node_ptr, MESH_DEMON_STATIC);
		break;

		case 1: storage.setAnimatedMesh(node_ptr, ANIMATED_DEMON_WALKING);
		break;

		case 2: storage.setAnimatedMesh(node_ptr, ANIMATED_DEMON_ATTACK);
		break;

		case 4: storage.setAnimatedMesh(node_ptr, ANIMATED_DEMON_JUMP);
		break;
	}
}

void RenderSystem::angelAnimationSwitch(Storage& storage, INode* node_ptr, const unsigned aniMode){ 
	switch (aniMode)
	{
		case 0: storage.setMesh(node_ptr, MESH_ANGEL_STATIC);
		break;

		case 1: storage.setAnimatedMesh(node_ptr, ANIMATED_ANGEL_WALKING);
		break;

		case 2: storage.setAnimatedMesh(node_ptr, ANIMATED_ANGEL_ATTACK);
		break;

		case 3: storage.setAnimatedMesh(node_ptr, ANIMATED_ANGEL_DEATH);
		break;
	}
}