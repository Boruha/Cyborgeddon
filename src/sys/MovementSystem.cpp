#include <sys/MovementSystem.hpp>

//TODO / CONSIDERACION: 
// 		1. Llamar 'Update()' a un método aux del System que se encarge de llamar a sus metodos.
//		2. Todas las entidades que sufran un cambio o una operacion, debería hacerlo igual los calculos.
//		por eso igual nos conviene crear metodos "Update_X.CMP / Update_X.ACCION".
//			2.1. Antes sería imperativo unificar todas entidades que hagan X acciones o tengan X cmp.
//			2.2. Renombrar los sistemas por la componente que trabajan o por el ambito: transformableSystem / velocitySystem, etc... (estudiable)

void MovementSystem::update(const std::unique_ptr<GameContext>& context) const {
	updatePlayerAndCamera(context->getPlayer(), context->getCamera());
	updateEnemies(context->getEnemies());
	updateBullets(context->getBullets());
	checkMaxDist_Bullet(context->getBullets());
}

// TODO: considerar la posicion del nodo para interpolar y la del transformable para mover las cosas en el juego
void MovementSystem::updatePlayerAndCamera(std::unique_ptr<EntityPlayer>& player, std::unique_ptr<EntityCamera>& camera) const {
	// Movimiento
	// CUIDADO -> lo que vemos en la escena es el NODO, si no modificamos player.nodo, no se va a mover nada
	player->transformable.position = player->node.getPosition();
    camera->transformable.position = camera->cameraNode.getPosition();


    player->transformable.position += player->velocity.velocity;
    player->collider.setPosition(player->transformable.position);

    camera->transformable.position += player->velocity.velocity;
    camera->cameraNode.updateTarget();

	player->node.setPosition(player->transformable.position);
    camera->cameraNode.setPosition(camera->transformable.position);

	/*
	 * Para evitar que se gire eternamente sobrepasando el limite representable (cosa improbable pero posible),
	 * siempre oscilamos entre (-360, 360] haciendo 1 comprobacion y 1 operacion.
	 * Otra opcion es corregir el valor si nos pasamos de 360 o si nos quedamos por debajo de 0,
	 * pero esto ejecuta siempre 2 comprobaciones y si es necesario, 1 operacion.
	*/
	if (player->transformable.rotation.y > 0)
		player->transformable.rotation.y -= 360;
	else
		player->transformable.rotation.y += 360;

	player->node.setRotation(player->transformable.rotation);

	// TODO: esto es una basura asi como esta hecho, arreglar en el futuro
	// DASH
	if(player->velocity.speed > 1)
	    player->velocity.speed -= 1.2f;
	if(player->velocity.speed < 1)
	    player->velocity.speed = 1;
	// TODO: las entidades deben tener velocidad maxima const, minima const y actual variable
}

// TODO: considerar la posicion del nodo para interpolar y la del transformable para mover las cosas en el juego
void MovementSystem::updateEnemies(const std::vector<std::unique_ptr<EntityEnemy>>& enemies) const {
	for (auto & enemy : enemies) {
		enemy->transformable.position = enemy->node.getPosition();

		enemy->transformable.position += enemy->velocity.velocity;
		enemy->collider.setPosition(enemy->transformable.position);

		enemy->node.setPosition(enemy->transformable.position);
	}
}

// TODO: considerar la posicion del nodo para interpolar y la del transformable para mover las cosas en el juego
void MovementSystem::updateBullets(const std::vector<std::unique_ptr<EntityBullet>>& bullets) const {
	for (auto & bullet : bullets) {
		bullet->transformable.position = bullet->node.getPosition();

		bullet->transformable.position += bullet->velocity.velocity;
		bullet->collider.setPosition(bullet->transformable.position);

		bullet->node.setPosition(bullet->transformable.position);
	}
}

// TODO: reconsiderar logica -> las balas nunca se frenan. Para evitar calculos se podria tener una variable
//  	 llamada "distancia recorrida" que fuese sumando speed en cada iteracion y que la condicion de muerte sea
//  	 que esa distancia sea mayor a la distancia maxima de vida de la bala
// BORU: Una consideración buenarda.
void MovementSystem::checkMaxDist_Bullet(const std::vector<std::unique_ptr<EntityBullet>>& bullets) const {
	for (auto & bullet : bullets) {
		if((bullet->start_pos - bullet->transformable.position).length() >= bullet->dead_dist)
			bullet->alive = false;
	}
}