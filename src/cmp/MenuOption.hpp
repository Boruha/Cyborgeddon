#pragma once

#include <cmp/Component.hpp>

struct MenuOption : public Component {
	explicit MenuOption(EntityType e_type, EntityID e_ID, int option, int maxOptions);

	[[nodiscard]] constexpr static std::string_view getName() {
		return "MenuOption";
	}

	friend std::ostream& operator<<(std::ostream& os, const MenuOption& opt);

	int option     { 0 };
	int maxOptions { 0 };
};