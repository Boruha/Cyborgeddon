#include <sys/MovementSystem.hpp>

// TODO: considerar la posicion del nodo para interpolar y la del transformable para mover las cosas en el juego
void MovementSystem::update(std::unique_ptr<EntityPlayer>& player) {
	// CUIDADO -> lo que vemos en la escena es el NODO, si no modificamos player.nodo, no se va a mover nada
	player->transformable.position = player->node.getPosition();

	player->velocity.velocity = player->velocity.direction.normalize() * player->velocity.speed;
	player->transformable.position += player->velocity.velocity;

	player->node.setPosition(player->transformable.position);
}

// TODO: considerar la posicion del nodo para interpolar y la del transformable para mover las cosas en el juego
void MovementSystem::update(const std::vector<std::unique_ptr<EntityEnemy>>& enemies) {
	for (auto & enemy : enemies) {
		enemy->transformable.position = enemy->node.getPosition();

		enemy->velocity.velocity = enemy->velocity.direction.normalize() * enemy->velocity.speed;
		enemy->transformable.position += enemy->velocity.velocity;

		enemy->node.setPosition(enemy->transformable.position);
	}
}