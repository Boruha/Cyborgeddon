
#include <man/GameManager.hpp>
#include <zconf.h>

#include <Engine/util/Math.hpp>
#include <glm/gtx/string_cast.hpp>

int main()
{
	GameManager gameManager {};

	gameManager.init();
	gameManager.run();
	gameManager.terminate();

	system(std::string(std::string("sudo pmap ") + std::string(std::to_string(getpid())) + std::string(" | tail -n 1")).c_str());

	return 0;
}