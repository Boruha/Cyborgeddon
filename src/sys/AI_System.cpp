#include <sys/AI_System.hpp>
#include <cmath>

void AI_System::update(EntityPlayer& player, EntityEnemy& enemy){

    float dist_x = player.node->getPosition().X - enemy.node->getPosition().X;
    float dist_z = player.node->getPosition().Z - enemy.node->getPosition().Z;
    float distance = std::sqrt( std::pow(std::abs(dist_x), 2) + std::pow(std::abs(dist_z), 2));


    if( distance > 45){
        enemy.ai_state = 1;
    }
    else{
        enemy.ai_state = 0;
        enemy.vDirection.Z = 0;
        enemy.vDirection.X = 0;
    }

    if(enemy.ai_state == 1){

        if(dist_x < 0)
            enemy.vDirection.X = -1;
        else 
            enemy.vDirection.X = 1;

        if(dist_z < 0)
            enemy.vDirection.Z = -1;
        else 
            enemy.vDirection.Z = 1;
        
        enemy.vDirection.normalize();

    }
}

