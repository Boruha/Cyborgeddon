#pragma once

#include <ent/Entities.hpp>
#include <src/man/EntityManager.hpp>

#include <vector>

struct CollisionSystem
{
    //Vamos a hacer diferentes updates: Enemigo con balas, Personaje con balas, Personaje con enemigo, Personaje con pared, etc.
    void update(std::unique_ptr<EntityPlayer>&, const std::vector<std::unique_ptr<EntityDoor>>&, const std::vector<std::unique_ptr<EntityKey>>&,
                const std::vector<std::unique_ptr<EntityEnemy>>&, const std::vector<std::unique_ptr<EntityBullet>>&, const std::vector<std::unique_ptr<EntityWall>>&);
    void update(std::unique_ptr<EntityPlayer>&, const std::vector<std::unique_ptr<EntityDoor>>&) const;
    void update(std::unique_ptr<EntityPlayer>&, const std::vector<std::unique_ptr<EntityWall>>&) const;
    void update(std::unique_ptr<EntityPlayer>&, const std::vector<std::unique_ptr<EntityKey>>&)  const;
    void update(const std::vector<std::unique_ptr<EntityEnemy>>&, const std::vector<std::unique_ptr<EntityBullet>>&)  const;
    void update(std::unique_ptr<EntityPlayer>&, const std::vector<std::unique_ptr<EntityEnemy>>&)  const;
};
