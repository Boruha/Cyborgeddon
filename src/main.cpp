#include <iostream>
#include <man/GameManager.hpp>

using std::cerr;
using std::endl;

int main()
{
	GameManager gameManager{};

	if(gameManager.init() == 1)
	{
		cerr << "Error en gameManager init" << endl;
		return 1;
	}

	gameManager.loop();


	return 0;
}