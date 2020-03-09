
#include <man/GameManager.hpp>

int main()
{
	GameManager gameManager {};

	gameManager.init();
	gameManager.run();
	gameManager.terminate();

	return 0;
}