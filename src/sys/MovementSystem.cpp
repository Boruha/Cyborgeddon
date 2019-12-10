#include <sys/MovementSystem.hpp>

void MovementSystem::update(const std::vector<std::unique_ptr<EntityPlayer>>& players) {
	for(auto & player : players)
	{
		// CUIDADO -> lo que vemos en la escena es el NODO, si no modificamos player.nodo, no se va a mover nada
		player->transformable.position = player->node.getPosition();

		player->velocity.velocity = player->velocity.direccion.normalize() * player->velocity.speed;
		player->transformable.position += player->velocity.velocity;

		player->node.setPosition(player->transformable.position);
	}
}