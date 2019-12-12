#include <sys/CollisionSystem.hpp>

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
*/

void CollisionSystem::update(std::unique_ptr<EntityPlayer>& player, const std::vector<std::unique_ptr<EntityDoor>>& doors, const std::vector<std::unique_ptr<EntityKey>>& keys)
{
	// La posicion del nodo es la que uso para saber si chocare eventualmente. La posicion que NO debo tocar aqui JAMAS
	// es transformable.position. UNICAMENTE modificar velocity.direction o velocity.speed temporalmente si es necesario
	player->node.setPosition(player->transformable.position + player->velocity.direction.normalize() * player->velocity.speed);

	// IMPORTANTE:  si puedo tocar dos llaves (o dos puertas) a la vez en una misma iteracion del bucle del juego,
	// 			 	las condiciones siguientes NO seran correctas. No poner puertas muy juntas y asi nos ahorramos
	// 				varias comprobaciones por bucle

	if(!player->key) {				// De momento solo compruebo llaves si NO tengo
		update(player, keys);   // Comprueba si el player choca con una puerta
	} else {						// De momento solo compruebo puertas si TENGO llaves en mi poder
		update(player, doors);  // Comprueba si el player choca con una llave
    }

	// Tras comprobar la colision devolvemos el nodo a su sitio. Ya se encargara el sistema de movimiento de modificar
	// las posiciones tanto de la componente transformable como del nodo
	player->node.setPosition(player->transformable.position);
}

void CollisionSystem::update(std::unique_ptr<EntityPlayer>& player, const std::vector<std::unique_ptr<EntityDoor>>& doors) const {
	for(auto & door : doors) {
		if(player->node.intersects(door->node)) {
			player->key = false; // player ha usado la llave que tenia
			door->open = true;  // esto provoca la "muerte" de la puerta
		}
	}
}

void CollisionSystem::update(std::unique_ptr<EntityPlayer>& player, const std::vector<std::unique_ptr<EntityKey>>& keys) const {
	for(auto & key : keys) {
		if(player->node.intersects(key->node)) {
			player->key = true; // player ahora sabe que tiene una llave, de momento son genericas
			key->taken = true; // esto provoca la "muerte" de la llave
		}
	}
}

// TODO: GENERALIZAR, SE REPITE CASI TODO (ideas: bool que "mata" entidades, herencias, )