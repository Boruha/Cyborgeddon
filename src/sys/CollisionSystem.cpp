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

void CollisionSystem::update(EntityPlayer& player, EntityPlayer& cube )
{
    //Obtenemos posicion de las entidades
    irr::core::vector3df NoCollision;


    if(player.node->getTransformedBoundingBox().intersectsWithBox(cube.node->getTransformedBoundingBox())){

        player.node->setPosition(NoCollision);
        // player.velocity.velocity.X = 1;
        // player.velocity.velocity.Z = 0;
        // player.node->setPosition(player.node->getPosition() - irr::core::vector3df(1,0,1));

         std::cout<<"COLISIONA"<<std::endl;
    }
    else{
        irr::core::vector3df NoCollision = player.node->getPosition();
    }


}


