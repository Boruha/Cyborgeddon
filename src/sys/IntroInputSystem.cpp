#include <sys/IntroInputSystem.hpp>

void IntroInputSystem::fixedUpdate(const Context &context, const float deltaTime) {

	if (context->getVideoIndex() > 0) {
		auto& menu_options = context->getComponents().getComponents<MenuOption>()[0];

		if (context->isKeyTextInput(KEY_LEFT)) {
			menu_options.option = (((menu_options.option - 1) % menu_options.maxOptions) + menu_options.maxOptions) % menu_options.maxOptions;
		    soundMessages.emplace_back(MENU_CHANGE_OPTION);
		}

		if (context->isKeyTextInput(KEY_RIGHT)) {
			menu_options.option = (((menu_options.option + 1) % menu_options.maxOptions) + menu_options.maxOptions) % menu_options.maxOptions;
		    soundMessages.emplace_back(MENU_CHANGE_OPTION);
		}

		if(context->isKeyTextInput(KEY_SPACE) || context->isKeyTextInput(KEY_INTRO))
		    if(menu_options.option == 0)
		        soundMessages.emplace_back(STOP_ALL_SOUNDS);
	}
}
