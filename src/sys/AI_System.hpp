#pragma once

#include <ent/Entities.hpp>
#include <sys/Systems.hpp>

struct AI_System
{
	void init() {  }
    void update(const std::vector<std::unique_ptr<EntityPlayer>>& players, const std::vector<std::unique_ptr<EntityEnemy>>& enemies);

	private:
		void chasing(const std::unique_ptr<EntityEnemy>& enemy, const float& distance);
		float getAngleAxisZ(const Vector3f& dir);
};
