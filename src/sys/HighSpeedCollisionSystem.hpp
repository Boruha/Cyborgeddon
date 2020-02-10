#pragma once

#include <sys/System.hpp>

struct EntityHitData;
struct Line;

struct HighSpeedCollisionSystem : System {
	void init() override {  }
	void update(const std::unique_ptr<GameContext> &context, float deltaTime) const override;
	void reset() override {  }

	void checkHit(const Line& bulletRay, const BoundingBox& box, EntityHitData& hitData) const;

	void damageEntity(const BulletData& bullet, CharacterData& character) const;
};