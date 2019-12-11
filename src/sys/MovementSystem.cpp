#include <sys/MovementSystem.hpp>

void MovementSystem::update(const std::vector<std::unique_ptr<EntityPlayer>>& players) {
	for(auto & player : players)
	{
		// CUIDADO -> lo que vemos en la escena es el NODO, si no modificamos player.nodo, no se va a mover nada
		player->transformable.position = player->node.getPosition();

        /*std::cout << "Speed: " << player->velocity.speed << std::endl;
        std::cout << "Normal X: " << player->velocity.direccion.normalize().x << std::endl;
        std::cout << "Normal Y: " << player->velocity.direccion.normalize().y << std::endl;
        std::cout << "Normal Z: " << player->velocity.direccion.normalize().z << std::endl;
        */

		player->velocity.velocity = player->velocity.direccion.normalize() * player->velocity.speed;

		/*std::cout << "X: " << player->velocity.velocity.x << std::endl;
        std::cout << "Y: " << player->velocity.velocity.y << std::endl;
        std::cout << "Z: " << player->velocity.velocity.z << std::endl;
        */
		player->transformable.position += player->velocity.velocity;

		player->node.setPosition(player->transformable.position);
	}
}