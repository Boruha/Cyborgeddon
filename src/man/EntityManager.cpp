#include <man/EntityManager.hpp>

void EntityManager::init()
{
    createPlayer();
    createEnemy(Vector3f(0, 0, 50));
    createCamera(Vector3f(0,50,-60));
}

void EntityManager::update(){
    checkShooting();
    deleteBullet();
}

void EntityManager::checkShooting(){
    for (auto & player : players) {
        if(player->shooting){
            createBullet(player->node.getPosition(), player->velocity.direccion, player->mode);
            player->shooting = false;
        }
    }
}

void EntityManager::cleanVectors() {
	entities.erase(entities.begin(), entities.end());
	players.erase(players.begin(), players.end());
	cameras.erase(cameras.begin(), cameras.end());
	bullets.erase(bullets.begin(), bullets.end());
}

void EntityManager::deleteBullet(){
    for(long unsigned int i=0; i<bullets.size(); ++i){
        if(bullets.at(i)->dead)
            bullets.erase(bullets.begin()+i);
    }
}


/*  CREATE ENTITIES  */

void EntityManager::createPlayer(const Vector3f& pos, const Vector3f& dim, const float& speed) {
    players.emplace_back(std::make_unique<EntityPlayer>(device, pos, dim, speed));
}

void EntityManager::createEnemy(const Vector3f& pos, const Vector3f& dim, const float& speed) {
	enemies.emplace_back(std::make_unique<EntityEnemy>(device, pos, dim, speed));
}

void EntityManager::createCamera(const Vector3f& pos, const Vector3f& target) {
    cameras.emplace_back(std::make_unique<EntityCamera>(device, pos, target));
}

void EntityManager::createBullet(const Vector3f& pos, const Vector3f& dir, const bool& type) {
    bullets.emplace_back(std::make_unique<EntityBullet>(device, pos, dir, type));
}

