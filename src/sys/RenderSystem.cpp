#include <sys/RenderSystem.hpp>

#include <glm/gtx/string_cast.hpp>
#include <util/MeshPaths.hpp>

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

		renderCmp_ptr->aniMode = message.animationMode;

		switch (entity.getType())
		{
			case PLAYER :	switch (message.animationMode)
						  	{
						  		case 0: storage.setAnimatedMesh(node_ptr, ANIMATED_DEX_STATIC);
								break;

								case 1: storage.setAnimatedMesh(node_ptr, ANIMATED_DEX_WALKING);
								break;

								case 2: storage.setAnimatedMesh(node_ptr, ANIMATED_DEX_WALKING_ATTACK);
								break;

								case 3: storage.setAnimatedMesh(node_ptr, ANIMATED_DEX_DEATH);
								break;

								case 4: storage.setAnimatedMesh(node_ptr, ANIMATED_DEX_DASHING);
								break;

								case 5: storage.setAnimatedMesh(node_ptr, ANIMATED_DEX_STATIC_ATTACK);
								break;
							}
			break;

			case ENEMY : /* do smth */
			break;

			case DOOR : /* do smth */
			break;	

			default :
			break;
		}
	}

	animationMessages.clear();
}