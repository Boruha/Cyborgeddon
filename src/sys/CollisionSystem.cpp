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

void CollisionSystem::update(const std::unique_ptr<GameContext>& context) const {

	// La posicion del nodo es la que uso para saber si chocare eventualmente. La posicion que NO debo tocar aqui JAMAS
	// es transformable.position. UNICAMENTE modificar velocity.direction o velocity.speed temporalmente si es necesario
	auto player = context->getPlayer().get();
	player->node.setPosition(player->transformable.position + player->velocity.direction.normalize() * player->velocity.speed);

	// IMPORTANTE:  si puedo tocar dos llaves (o dos puertas) a la vez en una misma iteracion del bucle del juego,
	// 			 	las condiciones siguientes NO seran correctas. No poner puertas muy juntas y asi nos ahorramos
	// 				varias comprobaciones por bucle

    update(context->getPlayer(), context->getKeys());   // Comprueba si el player choca con una llave
    update(context->getPlayer(), context->getDoors());  // Comprueba si el player choca con una puerta
    update(context->getEnemies(), context->getBullets());  // Comprueba si le damos al enemy con la bala
    update(context->getPlayer(), context->getEnemies()); //Comprueba si el player choca con enemy y pierde vida

	// Tras comprobar la colision devolvemos el nodo a su sitio. Ya se encargara el sistema de movimiento de modificar
	// las posiciones tanto de la componente transformable como del nodo
	player->node.setPosition(player->transformable.position);
}

void CollisionSystem::update(std::unique_ptr<EntityPlayer>& player, const std::vector<std::unique_ptr<EntityDoor>>& doors) const {
    bool colision = true;
	for(auto & door : doors) {
		if(player->node.intersects(door->node)) {
		    std::cout << "Necesitas la llave: " << door->type << std::endl;
            if(!player->owned_keys.empty()){         //TENGO LLAVES
                //Compruebo si el player tiene en su array de llaves el tipo de llave que abre la puerta
                for(int i = 0; i < (int)player->owned_keys.size(); i++){
                    if(door->type == player->owned_keys.at(i)){
                        std::cout << "La llave " << player->owned_keys.at(i) << " encaja en la cerradura: " << door->type << std::endl;
                        player->owned_keys.erase(player->owned_keys.begin() + i); // player ha usado la llave que tenia
                        door->type = -1;        //TODO: Variable de muerte
                        colision = false;
                        break;
                    }
                }
                if(colision){
                    player->velocity.direction = 0;
                }
            }
            else{
                player->velocity.direction = 0;
            }
		}
	}
}

void CollisionSystem::update(std::unique_ptr<EntityPlayer>& player, const std::vector<std::unique_ptr<EntityKey>>& keys) const {
	for(auto & key : keys) {
		if(player->node.intersects(key->node)) {
            std::cout << "Obtenida la llave: " << key->type << std::endl;
		    player->owned_keys.emplace_back(key->type); // Le ponemos la correspondiente llave a player en su inventario
            key->type = -1;                                 //TODO: Variable de muerte
		}
	}
}

// TODO: considerar matar al enemigo mas cercano a la bala y no el primero en el vector de enemigos
void CollisionSystem::update(const std::vector<std::unique_ptr<EntityEnemy>> & enemies, const std::vector<std::unique_ptr<EntityBullet>> & bullets) const {
    for(auto & enemy : enemies) {
        for(auto & bullet : bullets) {
        	if(bullet->dead)
        		continue;
            if(enemy->node.intersects(bullet->node)) {
                std::cout << "Enemigo alcanzado" << std::endl;
                enemy->ai_state = -1;                   //TODO: Variable de muerte
                bullet->dead = true;
            }
        }
    }
}

void CollisionSystem::update(std::unique_ptr<EntityPlayer> & player, const std::vector<std::unique_ptr<EntityEnemy>> & enemies) const {
    for(auto & enemy : enemies) {
        if(player->node.intersects(enemy->node)) {
            player->health--;
            std::cout << "Vidas restantes:" << player->health << std::endl;
            if(player->health <= 0){
                std::cout << "Has amochao" << std::endl;
            }
        }
    }
}

// TODO: GENERALIZAR, SE REPITE CASI TODO (ideas: bool que "mata" entidades, herencias, )