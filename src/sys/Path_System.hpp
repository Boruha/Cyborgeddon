#pragma once

#include <sys/System.hpp>
#include <ent/Graph.hpp>

struct Path_System : System
{
    void init() override;
    void update(const std::unique_ptr<GameContext> &context, float deltaTime) const override;
	void reset() override {  }

    std::vector<int> calculePath(const int, const int, const std::vector<MapNode>&);
    int nearestNode(vec3&, const std::vector<MapNode>&);

};
