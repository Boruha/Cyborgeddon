#pragma once

#include <sys/System.hpp>

struct HighSpeedCollisionSystem : System {
	void init() override {  }
	void update(const std::unique_ptr<GameContext>& context) const override;

	[[nodiscard]] bool intersects(const Vector3f &lastPos, const Vector3f &newPos, const BoundingBox &box) const;
	[[nodiscard]] bool intersects(const Vector3f& a, const Vector3f& b, const Vector3f& c, const Vector3f& d) const;
	[[nodiscard]] bool pointOnSegment(const Vector3f& a, const Vector3f& b, const Vector3f& c) const;
	[[nodiscard]] int segmentOrientation(const Vector3f& a, const Vector3f& b, const Vector3f& c) const;
};