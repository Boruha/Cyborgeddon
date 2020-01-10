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

	static void dynamicCollision(BoundingBox& box, Vector3f& velocity, BoundingBox& otherBox, int coord, const std::unique_ptr<GameContext>& context);
	static void staticCollision(BoundingBox& box, Vector3f& velocity, BoundingBox& other, int coord, const std::unique_ptr<GameContext>& context);

	static void fixCoord(BoundingBox& bounding, int coord);
	static void fixBox(BoundingBox& bounding);
	static void moveCoord(BoundingBox& bounding, float mov, int coord);
	static void moveBox(BoundingBox& bounding, const Vector3f& mov);

	[[nodiscard]] static bool intersects(const BoundingBox& bounding, const BoundingBox& other);

	struct TTypeFunction {
		void (*p_func)(BoundingBox& box, Vector3f& velocity, BoundingBox& other, const int coord, const std::unique_ptr<GameContext>& context);
	};

	const TTypeFunction typeFunctions[ColliderType::END_TYPE] // END_TYPE es el tamaño del array
	{
		{ dynamicCollision },
		{ staticCollision  }
	};
};
