#include <sys/CollisionSystem.hpp>
#include <algorithm>

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

void CollisionSystem::update(const std::unique_ptr<GameContext> &context) const {

	// La posicion del nodo es la que uso para saber si chocare eventualmente. La posicion que NO debo tocar aqui JAMAS
	// es transformable.position. UNICAMENTE modificar velocity.direction o velocity.speed temporalmente si es necesario
	const auto player = context->getPlayer().get();
	player->node.setPosition(player->transformable.position + player->velocity.velocity);

	// IMPORTANTE:  si puedo tocar dos llaves (o dos puertas) a la vez en una misma iteracion del bucle del juego,
	// 			 	las condiciones siguientes NO seran correctas. No poner puertas muy juntas y asi nos ahorramos
	// 				varias comprobaciones por bucle

    update(context->getPlayer(),  context->getKeys());   // Comprueba si el player choca con una llave
    update(context->getPlayer(),  context->getDoors());  // Comprueba si el player choca con una puerta
	update(context->getPlayer(),  context->getWalls());
	update(context->getEnemies(), context->getBullets());  // Comprueba si le damos al enemy con la bala
	update(context->getDoors(), context->getBullets());  // Comprueba si le damos al enemy con la bala
	update(context->getWalls(), context->getBullets());  // Comprueba si le damos al enemy con la bala
    update(context->getPlayer(), context->getEnemies()); //Comprueba si el player choca con enemy y pierde vida

	// Tras comprobar la colision devolvemos el nodo a su sitio. Ya se encargara el sistema de movimiento de modificar
	// las posiciones tanto de la componente transformable como del nodo
	player->collider.setPosition(player->transformable.position);
	player->node.setPosition(player->transformable.position);
}

void CollisionSystem::update(const std::unique_ptr<EntityPlayer> &player, const std::vector<std::unique_ptr<EntityDoor>> &doors) const {
	bool colision = true;
	for (const auto& door : doors) {
		if (player->node.intersects(door->node)) {
			if (!player->owned_keys.empty()) {         //TENGO LLAVES
				//Compruebo si el player tiene en su array de llaves el tipo de llave que abre la puerta
				for (int i = 0; i < (int) player->owned_keys.size(); i++) {
					if (door->type == player->owned_keys.at(i)) {
						player->owned_keys.erase(player->owned_keys.begin() + i); // player ha usado la llave que tenia
						door->type = -1;        //TODO: Variable de muerte
						colision = false;
						break;
					}
				}
				if (colision) {
					player->velocity.velocity = 0;
				}
			} else {
				player->velocity.velocity = 0;
			}
		}
	}
}

void CollisionSystem::update(const std::unique_ptr<EntityPlayer> &player, const std::vector<std::unique_ptr<EntityKey>> &keys) const {
	for (const auto& key : keys) {
		if (player->collider.intersects(key->collider)) {
			player->owned_keys.emplace_back(key->type); // Le ponemos la correspondiente llave a player en su inventario
			key->type = -1;                             //TODO: Variable de muerte
		}
	}
}

// TODO: considerar matar al enemigo mas cercano a la bala y no el primero en el vector de enemigos
void CollisionSystem::update(const std::vector<std::unique_ptr<EntityEnemy>> &enemies, const std::vector<std::unique_ptr<EntityBullet>> &bullets) const {
	for (const auto& bullet : bullets) {
		for (const auto& enemy : enemies) {
			if (enemy->collider.intersects(bullet->collider)) {
				enemy->ai_state = -1;                   //TODO: Variable de muerte
				bullet->dead = true;
				break;									// la bala muere, no seguimos comprobando con esa bala
			}
		}
	}
}

void CollisionSystem::update(const std::unique_ptr<EntityPlayer> &player, const std::vector<std::unique_ptr<EntityEnemy>> &enemies) const {
	for (const auto& enemy : enemies) {
		if (player->collider.intersects(enemy->collider)) {
			std::cout << enemy->collider.pos << std::endl;
			player->health--;
		}
	}
}

void CollisionSystem::update(const std::unique_ptr<EntityPlayer> &player, const std::vector<std::unique_ptr<EntityWall>> &walls) const {
	for (int i = 0; i < 3; ++i) {
		if (player->velocity.velocity[i] != 0) {
			player->collider.pos[i] += player->velocity.velocity[i];
			player->collider.box.min[i] += player->velocity.velocity[i];
			player->collider.box.max[i] += player->velocity.velocity[i];

			float offset {0};

			for (const auto& wall : walls) {
				if (player->collider.intersects(wall->collider)) {
					if (player->velocity.velocity[i] > 0)
						offset = wall->collider.box.min[i] - player->collider.box.max[i];
					else
						offset = wall->collider.box.max[i] - player->collider.box.min[i];

					player->velocity.velocity[i] += offset;
					player->collider.pos[i] += offset;
					player->collider.box.min[i] += offset;
					player->collider.box.max[i] += offset;
				}
			}
		}
	}
}


void CollisionSystem::update(const std::vector<std::unique_ptr<EntityDoor>> &doors, const std::vector<std::unique_ptr<EntityBullet>> &bullets) const {
	for (const auto& bullet : bullets) {
		for (const auto& door : doors) {
			if (door->collider.intersects(bullet->collider)) {
				bullet->velocity.velocity = 0;
				bullet->dead = true;
				break;					// la bala muere, no seguimos comprobando con otras puertas
			}
		}
	}
}

void CollisionSystem::update(const std::vector<std::unique_ptr<EntityWall>> &walls, const std::vector<std::unique_ptr<EntityBullet>> &bullets) const {
	for (const auto& bullet : bullets) {
		for (const auto& wall : walls) {
			if (wall->collider.intersects(bullet->collider)) {
				bullet->velocity.velocity = 0;
				bullet->dead = true;
				break;					// la bala muere, no seguimos comprobando con otras paredes
			}
		}
	}
}

// TODO: GENERALIZAR, SE REPITE CASI TODO (ideas: bool que "mata" entidades, herencias, )