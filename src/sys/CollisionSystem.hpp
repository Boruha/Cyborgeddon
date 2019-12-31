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
    void update(const EntityPlayer&, const std::vector<EntityWall>&) const;
    void update(std::vector<EntityEnemy>&, std::vector<EntityBullet>&)  const;
    void update(const std::vector<EntityDoor>&, std::vector<EntityBullet>&)  const;
    void update(const std::vector<EntityWall>&, std::vector<EntityBullet>&)  const;
    void update(EntityPlayer&, const std::vector<EntityEnemy>&)  const;

	static void dynamicCollision(BoundingBox& box, Physics& physics, const BoundingBox& otherBox, const std::unique_ptr<GameContext>& context);
	static void staticCollision(BoundingBox& box, Physics& physics, const BoundingBox& other, const std::unique_ptr<GameContext>& context);
	static void rayCollision(BoundingBox& box, Physics& physics, const BoundingBox& other, const std::unique_ptr<GameContext>& context);

	inline static void fixCoord(BoundingBox& bounding, int coord);
	inline static void fixBox(BoundingBox& bounding);
	inline static void moveCoord(BoundingBox& bounding, float mov, int coord);
	[[nodiscard]] inline static bool intersects(const BoundingBox &bounding, const BoundingBox &other);

	struct TTypeFunction {
		void (*p_func)(BoundingBox& box, Physics& physics, const BoundingBox& other, const std::unique_ptr<GameContext>& context);
	};

	// IMPORTANTE : para acceder a este array hay que : stateFunctions[STATE DEL ENEMY].p_func(parametros)
	// 				Hay que respetar SI O SI el orden de AI_STATE con sus funciones asociadas
	//				Si el orden de AI_STATE es patrol - pursue - X - Y, el orden de las funciones tiene que ser
	//				obligatoriamente patrolFunc - pursueFunc - Xfunc - Yfunc.
	//				Si no se respeta esto, los enemigos se comportaran de manera erratica
	const TTypeFunction typeFunctions[ColliderType::END_TYPE] // END_STATE es el tamaño del array
	{
		{ dynamicCollision },
		{ staticCollision  },
		{ rayCollision	   }
	};

};
