#include <sys/CollisionSystem.hpp>
#include <algorithm>
#include <SunlightEngine/Vector3.hpp>

//Estrategias para implementar la colision

/*         1-Lenyendo teclas
 *
 * Si pulsamos W -> player.velocity.velocity.Z =  1
 * Si pulsamos A -> player.velocity.velocity.Z = -1
 * Si pulsamos S -> player.velocity.velocity.X =  1
 * Si pulsamos D -> player.velocity.velocity.Z = -1
 */

/*         2-Comprobando la cara del cuadrado que esta chochando
 *
 * Si choca la cara delantera  -> player.velocity.velocity.Z =  1
 * Si choca la cara trasera    -> player.velocity.velocity.Z = -1
 * Si chocha la cara izquierda -> player.velocity.velocity.X =  1
 * Si choca la cara derecha    -> player.velocity.velocity.Z = -1
 */

/*         3-(la mas viable para mi) - Guardar ultima posicion
 * Consistiría en almacenar la ultima posicion anterior a la colision y restaurarla
 * en caso de que haya una. Ahorraria mogollon de comprobaciones tontas.
 */

/*
void MovementSystem::move(const std::vector<std::unique_ptr<EntityPlayer>>& players) {
    for(auto & player : players)
    {
        // CUIDADO -> lo que vemos en la escena es el NODO, si no modificamos player.nodo, no se va a mover nada
        player->transformable.position = player->node.getPosition();

        player->velocity.velocity = player->velocity.direccion.normalize() * player->velocity.speed;
        player->transformable.position += player->velocity.velocity;

        player->node.setPosition(player->transformable.position);
    }
}
*/

// TODO: separar colisiones estaticas (donde se corrige posicion) de las simples (solo comprobamos si colisiona)

void CollisionSystem::update(const std::unique_ptr<GameContext> &context) const {
	// IMPORTANTE:  si puedo tocar dos llaves (o dos puertas) a la vez en una misma iteracion del bucle del juego,
	// 			 	las condiciones siguientes NO seran correctas. No poner puertas muy juntas y asi nos ahorramos
	// 				varias comprobaciones por bucle

	// Antes de comoprobar cualquier tipo de colision con un collider que NO sea estatico, hay que asegurarse de que
	// la caja de colision se encuentra en su sitio haciendo collider.fixBox()
	// Después de cualquier metodo que pueda modificar la caja de colisiones (comprobando con otros colliders estaticos)
	// hay que volver a hacef collider.fixBox()

	// Ejemplo: player -> fix, tras comprobar colision player-pared es posible que la caja de colisiones haya sufrido
	// 			alguna modificación. CUALQUIER metodo estatico al final debe SÍ O SÍ hacer un fix de todos los colliders
	// 			que haya tocado. Para ahorrar calculos, calcular las colisiones estaticas primero

	context->getPlayer()->collider->fixBox();

	for(const auto& enemy : context->getEnemies()) {
		enemy->collider->fixBox();
	}
	for(const auto& bullet : context->getBullets()) {
		bullet->collider->fixBox();
	}

    update(context->getPlayer(),  context->getDoors());  	// Comprueba si el player choca con una puerta (colision estatica)
    update(context->getPlayer(),  context->getWalls());  	// Comprueba si el player choca con una pared (colision estatica)
    update(context->getPlayer(),  context->getKeys());   	// Comprueba si el player choca con una llave
	update(context->getEnemies(), context->getBullets());  	// Comprueba si le damos al enemy con la bala
	update(context->getDoors(),   context->getBullets());  	// Comprueba si le damos a una puerta con la bala
	update(context->getWalls(),   context->getBullets());  	// Comprueba si le damos a una pared con la bala
    update(context->getPlayer(),  context->getEnemies()); 	// Comprueba si el player choca con enemy y pierde vida
}

void CollisionSystem::update(const std::unique_ptr<EntityPlayer> &player, const std::vector<std::unique_ptr<EntityDoor>> &doors) const {
	for (const auto& door : doors) {
		for (int i = 0; i < 3; ++i) {
			if (player->velocity->velocity[i] != 0) {
				player->collider->moveCoord(player->velocity->velocity[i], i);

				float offset {0};

					if (player->collider->intersects(*door->collider)) {
						if (player->my_keys.at(door->lock->ID)) {
							player->my_keys.at(door->lock->ID) = false;
							door->alive.alive = false;
							break;
						} else {
							if (player->velocity->velocity[i] > 0)
								offset = door->collider->box.min[i] - player->collider->box.max[i];
							else
								offset = door->collider->box.max[i] - player->collider->box.min[i];

							player->velocity->velocity[i] += offset;
							player->collider->moveCoord(offset, i);
						}
					}

			}
		}
		// tras comprobar la colision con la puerta que puede ser estatica, corregimos collider
		player->collider->fixBox();
	}
}

void CollisionSystem::update(const std::unique_ptr<EntityPlayer> &player, const std::vector<std::unique_ptr<EntityKey>> &keys) const {
	for (const auto& key : keys) {
		if (player->collider->intersects(*key->collider)) {
			player->my_keys.at(key->lock->ID) = true; // Le ponemos la correspondiente llave a player en su inventario
			key->alive.alive = false;                             //TODO: Variable de muerte
			break;											// no necesitamos seguir comprobando llaves
		}
	}
}

// TODO: considerar matar al enemigo mas cercano a la bala y no el primero en el vector de enemigos
void CollisionSystem::update(const std::vector<std::unique_ptr<EntityEnemy>> &enemies, const std::vector<std::unique_ptr<EntityBullet>> &bullets) const {
	for (const auto& bullet : bullets) {
		for (const auto& enemy : enemies) {
			if (enemy->collider->intersects(*bullet->collider)) {
				enemy->alive.alive = false;                   //TODO: Variable de muerte
				bullet->alive.alive = false;
				break;									// la bala muere, no seguimos comprobando con esa bala
			}
		}
	}
}

void CollisionSystem::update(const std::unique_ptr<EntityPlayer> &player, const std::vector<std::unique_ptr<EntityEnemy>> &enemies) const {
	for (const auto& enemy : enemies) {
		if (player->collider->intersects(*enemy->collider)) {
			player->health--;
		}
	}
}

void CollisionSystem::update(const std::unique_ptr<EntityPlayer> &player, const std::vector<std::unique_ptr<EntityWall>> &walls) const {
	for (const auto& wall : walls) {
		for(int i = 0; i < 3; ++i) {
			if (player->velocity->velocity[i] != 0) {
				player->collider->moveCoord(player->velocity->velocity[i], i);

				float offset {0};

				if (player->collider->intersects(*wall->collider)) {
					if (player->velocity->velocity[i] > 0)
						offset = wall->collider->box.min[i] - player->collider->box.max[i];
					else
						offset = wall->collider->box.max[i] - player->collider->box.min[i];

					player->velocity->velocity[i] += offset;
					player->collider->moveCoord(offset, i);
				}
			}
		}
		// tras la colision con la pared que es estatica, corregimos collider
		player->collider->fixBox();
	}
}

void CollisionSystem::update(const std::vector<std::unique_ptr<EntityDoor>> &doors, const std::vector<std::unique_ptr<EntityBullet>> &bullets) const {
	for (const auto& bullet : bullets) {
		for (const auto& door : doors) {
			if (door->collider->intersects(*bullet->collider)) {
				bullet->velocity->velocity = 0;
				bullet->alive.alive = false;
				break;					// la bala muere, no seguimos comprobando con otras puertas
			}
		}
	}
}

void CollisionSystem::update(const std::vector<std::unique_ptr<EntityWall>> &walls, const std::vector<std::unique_ptr<EntityBullet>> &bullets) const {
	for (const auto& bullet : bullets) {
		for (const auto& wall : walls) {
			if (wall->collider->intersects(*bullet->collider)) {
				bullet->velocity->velocity = 0;
				bullet->alive.alive = false;
				break;					// la bala muere, no seguimos comprobando con otras paredes
			}
		}
	}
}

// TODO: GENERALIZAR, SE REPITE CASI TODO (ideas: bool que "mata" entidades, herencias, )