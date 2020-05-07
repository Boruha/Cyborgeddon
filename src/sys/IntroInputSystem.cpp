#include <sys/IntroInputSystem.hpp>



void IntroInputSystem::fixedUpdate(const Context &context, const float deltaTime) {

	if (context->getVideoIndex() > 0) {
		auto& menu_options = context->getComponents().getComponents<MenuOption>()[0];

		if (context->isKeyTextInput(KEY_LEFT))
			menu_options.option = (menu_options.option - 1) % menu_options.maxOptions;
		if (context->isKeyTextInput(KEY_RIGHT))
			menu_options.option = (menu_options.option + 1) % menu_options.maxOptions;
	}
}
