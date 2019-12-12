#include <sys/MovementSystem.hpp>

// TODO: considerar la posicion del nodo para interpolar y la del transformable para mover las cosas en el juego
void MovementSystem::update(std::unique_ptr<EntityPlayer>& player) {
	// CUIDADO -> lo que vemos en la escena es el NODO, si no modificamos player.nodo, no se va a mover nada
	player->transformable.position = player->node.getPosition();

	player->velocity.velocity = player->velocity.direction.normalize() * player->velocity.speed;
	player->transformable.position += player->velocity.velocity;

	player->node.setPosition(player->transformable.position);

	// TODO: esto es una basura asi como esta hecho, arreglar en el futuro
	// DASH
	if(player->velocity.speed > 1)
	    player->velocity.speed -= 1.2f;
	if(player->velocity.speed < 1)
	    player->velocity.speed = 1;
	// TODO: las entidades deben tener velocidad maxima const, minima const y actual variable
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

// TODO: considerar la posicion del nodo para interpolar y la del transformable para mover las cosas en el juego
void MovementSystem::update(const std::vector<std::unique_ptr<EntityBullet>>& bullets) {
	for (auto & bullet : bullets) {
		bullet->transformable.position = bullet->node.getPosition();

		bullet->velocity.velocity = bullet->velocity.direction.normalize() * bullet->velocity.speed;
		bullet->transformable.position += bullet->velocity.velocity;

		bullet->node.setPosition(bullet->transformable.position);
	}
}

// TODO: reconsiderar logica -> las balas nunca se frenan. Para evitar calculos se podria tener una variable
//  	 llamada "distancia recorrida" que fuese sumando speed en cada iteracion y que la condicion de muerte sea
//  	 que esa distancia sea mayor a la distancia maxima de vida de la bala
void MovementSystem::checkMaxDist_Bullet(const std::vector<std::unique_ptr<EntityBullet>>& bullets){
	for (auto & bullet : bullets) {
		float distance = Vector2f(bullet->start_pos.x - bullet->transformable.position.x, bullet->start_pos.z - bullet->transformable.position.z).length();
		if(distance >= bullet->dead_dist)
			bullet->dead = true;
	}
}
