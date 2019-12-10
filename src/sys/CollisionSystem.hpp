#pragma once

#include <ent/Entities.hpp>

#include <vector>

struct CollisionSystem
{
    //Vamos a hacer diferentes updates: Enemigo con balas, Personaje con balas, Personaje con enemigo, Personaje con pared, etc.
    void update(EntityPlayer& player, EntityPlayer& cube, EntityKey& key);
    void collisionPlayerKey(EntityPlayer& player, EntityKey& key);

    irr::core::vector3df NoCollision;
};
