#pragma once

#include <sys/System.hpp>

struct EntityHitData;
struct Line;

struct HighSpeedCollisionSystem : System {
	void init() override {  }
	void update(const Context &context, float deltaTime) override;
	void reset() override {  }

	private:

		void checkHit(const Line& bulletRay, const BoundingBox& box, EntityHitData& hitData) const;
		void damageEntity(const BulletData& bullet, CharacterData& character) const;
};