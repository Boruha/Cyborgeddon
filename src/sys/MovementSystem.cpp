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

    player->transformable->position += player->velocity->velocity;
    camera->transformable->position += camera->velocity->velocity;

	/*
	 * Para evitar que se gire eternamente sobrepasando el limite representable (cosa improbable pero posible),
	 * siempre oscilamos entre (-360, 360] haciendo 1 comprobacion y 1 operacion.
	 * Otra opcion es corregir el valor si nos pasamos de 360 o si nos quedamos por debajo de 0,
	 * pero esto ejecuta siempre 2 comprobaciones y si es necesario, 1 operacion.
	*/
	if (player->transformable->rotation.y > 0)
		player->transformable->rotation.y -= 360;
	else
		player->transformable->rotation.y += 360;

	// TODO: esto es una basura asi como esta hecho, arreglar en el futuro
	// DASH
	if(player->velocity->speed > 1)
	    player->velocity->speed -= 1.2f;
	if(player->velocity->speed < 1)
	    player->velocity->speed = 1;
	// TODO: las entidades deben tener velocidad maxima const, minima const y actual variable
}

// TODO: considerar la posicion del nodo para interpolar y la del transformable para mover las cosas en el juego
void MovementSystem::updateEnemies(const std::vector<std::unique_ptr<EntityEnemy>>& enemies) const {
	for (auto & enemy : enemies)
		enemy->transformable->position += enemy->velocity->velocity;
}

// TODO: considerar la posicion del nodo para interpolar y la del transformable para mover las cosas en el juego
void MovementSystem::updateBullets(const std::vector<std::unique_ptr<EntityBullet>>& bullets) const {
	for (auto & bullet : bullets)
		bullet->transformable->position += bullet->velocity->velocity;
}

// TODO: reconsiderar logica -> las balas nunca se frenan. Para evitar calculos se podria tener una variable
//  	 llamada "distancia recorrida" que fuese sumando speed en cada iteracion y que la condicion de muerte sea
//  	 que esa distancia sea mayor a la distancia maxima de vida de la bala
// BORU: Una consideración buenarda.
void MovementSystem::checkMaxDist_Bullet(const std::vector<std::unique_ptr<EntityBullet>>& bullets) const {
	for (auto & bullet : bullets) {
		bullet->distance_left -= bullet->velocity->speed;
		if(bullet->distance_left <= 0)
			bullet->alive.alive = false;
	}
}