#include <sys/MovementSystem.hpp>

void MovementSystem::update(EntityPlayer& player)
{
	// CUIDADO -> lo que vemos en la escena es el NODO, si no modificamos player.nodo, no se va a mover nada
	player.transformable.position = player.node->getPosition();
	player.transformable.position += player.velocity.velocity;
	player.node->setPosition(player.transformable.position);
}