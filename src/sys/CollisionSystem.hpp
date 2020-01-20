#pragma once

#include <src/man/EntityManager.hpp>
#include <sys/System.hpp>

struct CollisionSystem : System
{
	void init() override {  }
    void update(const std::unique_ptr<GameContext> &context, float deltaTime) const override;

private:

	void dynamicCollision(BoundingBox& box, Vector3f& velocity, BoundingBox& otherBox, const std::unique_ptr<GameContext>& context) const;
	void staticCollision(BoundingBox& box, Vector3f& velocity, const BoundingBox& other, int coord) const;

	void fixCoord(BoundingBox& bounding, int coord) const;
	void fixBox(BoundingBox& bounding) const;
	void moveCoord(BoundingBox& bounding, float mov, int coord) const;
	void moveBox(BoundingBox& bounding, const Vector3f& mov) const;
	void setBox(BoundingBox& bounding, const Vector3f& pos) const;
	void setCoord(BoundingBox& bounding, const Vector3f& pos, int coord) const;

	[[nodiscard]] bool intersects(const BoundingBox& bounding, const BoundingBox& other) const;

};
