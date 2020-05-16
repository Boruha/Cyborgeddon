#include <sys/PauseInputSystem.hpp>

void PauseInputSystem::fixedUpdate(const Context &context, const float deltaTime) {
	auto& menu_options = context->getComponents().getComponents<MenuOption>()[0];

	if (context->isKeyTextInput(KEY_UP)) {
		menu_options.option = (((menu_options.option - 1) % menu_options.maxOptions) + menu_options.maxOptions) % menu_options.maxOptions;
		soundMessages.emplace_back(MENU_CHANGE_OPTION);
	}

	if (context->isKeyTextInput(KEY_DOWN)) {
		menu_options.option = (((menu_options.option + 1) % menu_options.maxOptions) + menu_options.maxOptions) % menu_options.maxOptions;
		soundMessages.emplace_back(MENU_CHANGE_OPTION);
	}
}
