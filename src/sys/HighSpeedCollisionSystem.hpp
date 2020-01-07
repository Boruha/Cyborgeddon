#pragma once

#include <sys/System.hpp>

struct HighSpeedCollisionSystem : System {
	void init() override {  }
	void update(const std::unique_ptr<GameContext>& context) const override;

	[[nodiscard]] static bool intersects(const Vector3f& a, const Vector3f& b, const Vector3f& c, const Vector3f& d);
	[[nodiscard]] static bool pointOnSegment(const Vector3f& a, const Vector3f& b, const Vector3f& c);
	[[nodiscard]] static int segmentOrientation(const Vector3f& a, const Vector3f& b, const Vector3f& c);
};