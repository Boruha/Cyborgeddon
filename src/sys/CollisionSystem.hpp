#pragma once

#include <ent/Entities.hpp>
#include <src/man/EntityManager.hpp>

#include <vector>

struct CollisionSystem
{
    //Vamos a hacer diferentes updates: Enemigo con balas, Personaje con balas, Personaje con enemigo, Personaje con pared, etc.
    void update(const std::vector<std::unique_ptr<EntityPlayer>>&, const std::vector<std::unique_ptr<EntityDoor>>&);
 //   void collisionPlayerKey(EntityPlayer& player, EntityKey& key);

    irr::core::vector3df NoCollision;

};
