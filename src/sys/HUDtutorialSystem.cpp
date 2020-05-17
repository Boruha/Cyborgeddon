#include <sys/HUDtutorialSystem.hpp>

void HUDtutorialSystem::fixedUpdate(const Context &context, const float deltaTime) {
	context->getEngine().clear(Color(BLACK), true);
}
