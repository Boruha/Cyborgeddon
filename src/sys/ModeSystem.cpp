#include <sys/ModeSystem.hpp>

void ModeSystem::init(){}

// TODO: si no apretamos ninguna tecla esta comprobacion comera tiempo de ejecucion innecesariamente
//  	 llevar a InputSystem cuando se pueda
void ModeSystem::update(const std::unique_ptr<EntityPlayer>& player){
    if(player->mode)
        player->node.setTexture("./img/textures/testing/testing_angel.jpg");
    else
        player->node.setTexture("./img/textures/testing/testing_demon.jpg");
        
}

