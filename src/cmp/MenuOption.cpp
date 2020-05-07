#include <cmp/MenuOption.hpp>

#include <ostream>

MenuOption::MenuOption(const EntityType type, const EntityID e_ID, const int _option, const unsigned _maxOptions)
	: Component(type, e_ID), option(_option), maxOptions(_maxOptions) { }


std::ostream& operator<<(std::ostream& os, const MenuOption& opt)
{
	opt.print(os, MenuOption::getName())
		<< "\n\tNumOption: " << opt.option;

	return os;
}