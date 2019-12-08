#include <sys/MovementSystem.hpp>

void MovementSystem::update(const std::vector<std::unique_ptr<EntityPlayer>>& players) {
	for(auto & player : players)
	{
		// CUIDADO -> lo que vemos en la escena es el NODO, si no modificamos player.nodo, no se va a mover nada
		irr::core::vector3df irr_vec = player->node->getPosition();
		player->transformable.position = Vector3f(irr_vec.X, irr_vec.Y, irr_vec.Z);

		player->velocity.velocity = player->velocity.direccion.normalize() * player->velocity.speed;

		player->transformable.position += player->velocity.velocity;
		Vector3f sun_vec = player->transformable.position;
		player->node->setPosition(irr::core::vector3df(sun_vec.x, sun_vec.y, sun_vec.z));
	}
}