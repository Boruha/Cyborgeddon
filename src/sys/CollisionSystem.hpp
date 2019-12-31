#pragma once

#include <ent/Entities.hpp>
#include <src/man/EntityManager.hpp>
#include <sys/System.hpp>

#include <vector>

struct CollisionSystem : System
{
	void init() override {  }
    void update(const std::unique_ptr<GameContext>& context) const override;

private:

	static void dynamicCollision(BoundingBox& box, Physics& physics, BoundingBox& otherBox, int coord, const std::unique_ptr<GameContext>& context);
	static void staticCollision(BoundingBox& box, Physics& physics, BoundingBox& other, int coord, const std::unique_ptr<GameContext>& context);
	static void rayCollision(BoundingBox& box, Physics& physics, BoundingBox& other, int coord, const std::unique_ptr<GameContext>& context);

	static void fixCoord(BoundingBox& bounding, int coord);
	static void fixBox(BoundingBox& bounding);
	static void moveCoord(BoundingBox& bounding, float mov, int coord);
	static void moveBox(BoundingBox &bounding, const Vector3f& mov);

	[[nodiscard]] static bool intersects(const BoundingBox &bounding, const BoundingBox &other);

	struct TTypeFunction {
		void (*p_func)(BoundingBox& box, Physics& physics, BoundingBox& other, const int coord, const std::unique_ptr<GameContext>& context);
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
