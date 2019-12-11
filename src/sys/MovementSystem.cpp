#include <sys/MovementSystem.hpp>
#include <iostream>

// TODO: considerar la posicion del nodo para interpolar y la del transformable para mover las cosas en el juego
void MovementSystem::update(const std::vector<std::unique_ptr<EntityPlayer>>& players) {
	for (auto &player : players) {
		// CUIDADO -> lo que vemos en la escena es el NODO, si no modificamos player.nodo, no se va a mover nada
		player->transformable.position = player->node.getPosition();

		player->velocity.velocity = player->velocity.direccion.normalize() * player->velocity.speed;
		player->transformable.position += player->velocity.velocity;

		player->node.setPosition(player->transformable.position);
	}
}

// TODO: considerar la posicion del nodo para interpolar y la del transformable para mover las cosas en el juego
void MovementSystem::update(const std::vector<std::unique_ptr<EntityEnemy>>& enemies) {
	for (auto & enemy : enemies) {
		enemy->transformable.position = enemy->node.getPosition();

		enemy->velocity.velocity = enemy->velocity.direccion.normalize() * enemy->velocity.speed;
		enemy->transformable.position += enemy->velocity.velocity;

		enemy->node.setPosition(enemy->transformable.position);
	}
}

// TODO: considerar la posicion del nodo para interpolar y la del transformable para mover las cosas en el juego
void MovementSystem::update(const std::vector<std::unique_ptr<EntityBullet>>& bullets) {
	for (auto & bullet : bullets) {
		bullet->transformable.position = bullet->node.getPosition();

		bullet->velocity.velocity = bullet->velocity.direccion.normalize() * bullet->velocity.speed;
		bullet->transformable.position += bullet->velocity.velocity;

		bullet->node.setPosition(bullet->transformable.position);
	}
}

void MovementSystem::checkMaxDist_Bullet(const std::vector<std::unique_ptr<EntityBullet>>& bullets){
	float actual_dist_x = 0.f;
	float actual_dist_z = 0.f;
	float actual_dist = 0.f;

	for (auto & bullet : bullets) {
		actual_dist_x = bullet->start_pos.x - bullet->transformable.position.x;
		actual_dist_z = bullet->start_pos.z - bullet->transformable.position.z;
		actual_dist =  sqrt(actual_dist_x * actual_dist_x + actual_dist_z * actual_dist_z);

		if(actual_dist >= bullet->dead_dist)
			bullet->dead = true;
	}
}
