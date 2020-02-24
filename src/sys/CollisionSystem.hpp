#pragma once

#include <sys/System.hpp>

struct CollisionSystem : System {
	void init() override {  }
    void update(const Context &context, float deltaTime) override;
	void reset() override {  }

private:

	void dynamicCollision(BoundingBox& movingBox, vec3& velocity, BoundingBox& otherBox, const Context& context) const;
	void staticCollision(BoundingBox& movingBox, vec3& velocity, const BoundingBox& otherBox, int coord) const;

	void fixCoord(BoundingBox& bounding, int coord) const;
	void fixBox(BoundingBox& bounding) const;
	void moveCoord(BoundingBox& bounding, float mov, int coord) const;
	void moveBox(BoundingBox& bounding, const vec3& mov) const;
	void setBox(BoundingBox& bounding, const vec3& pos) const;
	void setCoord(BoundingBox& bounding, const vec3& pos, int coord) const;

	[[nodiscard]] bool intersects(const BoundingBox& bounding, const BoundingBox& other) const;
};
