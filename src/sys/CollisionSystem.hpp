#pragma once

#include <ent/Entities.hpp>
#include <src/man/EntityManager.hpp>
#include <sys/System.hpp>

#include <vector>

struct CollisionSystem : System
{
	void init() override {  }
    //Vamos a hacer diferentes updates: Enemigo con balas, Personaje con balas, Personaje con enemigo, Personaje con pared, etc.

    void update(const std::unique_ptr<GameContext>& context) const override;

private:
    void update(const std::unique_ptr<EntityPlayer>&, std::vector<EntityDoor>&) const;
    void update(const std::unique_ptr<EntityPlayer>&, const std::vector<EntityWall>&) const;
    void update(const std::unique_ptr<EntityPlayer>&, std::vector<EntityKey>&)  const;
    void update(std::vector<EntityEnemy>&, std::vector<EntityBullet>&)  const;
    void update(const std::vector<EntityDoor>&, std::vector<EntityBullet>&)  const;
    void update(const std::vector<EntityWall>&, std::vector<EntityBullet>&)  const;
    void update(const std::unique_ptr<EntityPlayer>&, const std::vector<EntityEnemy>&)  const;

    void updatePlayerWall(const std::unique_ptr<EntityPlayer>& player, const BoundingBox& box) const;
	void updatePlayerDoor(const std::unique_ptr<EntityPlayer>& player, const std::unique_ptr<Entity>& entity) const;
	void updatePlayerKey(const std::unique_ptr<EntityPlayer>& player, const std::unique_ptr<Entity>& entity) const;

	void updatePlayerVsStaticBounding(BoundingBox& player, Physics& physics, const BoundingBox& box) const;

	void fixCoord(BoundingBox &bounding, const int& coord) const;
	void fixBox(BoundingBox& bounding) const;
	void moveCoord(BoundingBox& bounding, const float& mov, const int& coord) const;
	[[nodiscard]] bool intersects(const BoundingBox &bounding, const BoundingBox &other) const;

	void updatePlayerVsDynamicBounding(BoundingBox &playerBox, BoundingBox &otherBox, const std::unique_ptr<GameContext> &context) const;
};
