
#include <man/GameManager.hpp>

int main()
{
	GameManager gameManager;

	gameManager.init();
	gameManager.loop();

	return 0;
}