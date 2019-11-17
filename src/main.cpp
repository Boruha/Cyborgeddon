#include <iostream>
#include "ent/gilardo.h"

int main()
{
	gilardo* gil = new gilardo();
	std::cout << "FUNCIONO" << std::endl;
	std::cout << "REPUTASO" << std::endl;
	gil->insulto();
	return 0;
}