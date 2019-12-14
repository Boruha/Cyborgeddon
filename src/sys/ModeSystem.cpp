#include <sys/ModeSystem.hpp>

void ModeSystem::init(){}

void ModeSystem::update(const std::unique_ptr<EntityPlayer>& player){
    if(player->mode)
        player->node.setTexture("./img/textures/testing/testing_angel.jpg");
    else
        player->node.setTexture("./img/textures/testing/testing_demon.jpg");
        
}

