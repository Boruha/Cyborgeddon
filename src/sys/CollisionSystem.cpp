#include <sys/CollisionSystem.hpp>
#include <iostream>

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

void CollisionSystem::update(const std::vector<std::unique_ptr<EntityPlayer>>& players, const std::vector<std::unique_ptr<EntityDoor>>& doors)
{
    for(auto & player :players){
        for(auto & door : doors){
            if(player->node.getTransformedBoundingBox().intersectsWithBox(door->node.getTransformedBoundingBox())){
                std::cout<<"COLISIONA" <<std::endl;
            }
            else{
                std::cout<<"NO COLISIONA "<<std::endl;
            }
        }
    }


    /*
    if(player.node->getTransformedBoundingBox().intersectsWithBox(door.node->getTransformedBoundingBox())){
        player.node->setPosition(NoCollision);

        player.key = false;
        std::cout<<"Llave dejada: "<< player.key << std::endl;
    }
    else{
        NoCollision =player.node->getPosition();
        //std::cout<<player.node->getPosition().X<<std::endl;
    }

    collisionPlayerKey(player, key);
     */
}

/*
void CollisionSystem::collisionPlayerKey(EntityPlayer& player, EntityKey& key){
    if(player.node->getTransformedBoundingBox().intersectsWithBox(key.node->getTransformedBoundingBox())){
        player.key = true;
        std::cout<<"Llave cogida: "<< player.key << std::endl;

    }
}

*/

