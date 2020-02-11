#pragma once

#include <sys/System.hpp>
#include <ent/Graph.hpp>

#define SIZE_GRAPH 5

struct Path_System : System
{
    void init() override;
    void update(const std::unique_ptr<GameContext> &context, float deltaTime) const override;
	void reset() override {  }

    std::vector<int> calculePath(int, int);
    int nearestNode(vec3&);

    std::vector<MapNode> graph;
};
