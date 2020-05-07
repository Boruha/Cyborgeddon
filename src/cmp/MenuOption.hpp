#pragma once

#include <cmp/Component.hpp>

struct MenuOption : public Component {
	explicit MenuOption(EntityType e_type, EntityID e_ID, int option, unsigned maxOptions);

	[[nodiscard]] constexpr static std::string_view getName() {
		return "MenuOption";
	}

	friend std::ostream& operator<<(std::ostream& os, const MenuOption& opt);

	int option          { -1 }; // -1 -> no hay opciones | 0,1,2,3... -> opciones
	unsigned maxOptions {  0 };
};