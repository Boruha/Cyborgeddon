#pragma once

#include <cmp/Component.hpp>
#include <util/ComponentConstants.hpp>
#include <set>
#include <tuple>

using glm::vec2;

struct GraphNode {

	using pair = std::pair<float, GraphNode*>;

	explicit GraphNode(const vec2& p, char c = '0');

	vec2 position            {         };
	GraphNode* parent        { nullptr };
	unsigned numConns        {    0    };
	char id                  {         };
	bool alreadyVisited [MAX_GRAPH_CONN];
	pair conns          [MAX_GRAPH_CONN];
};

inline bool operator < (const GraphNode& a, const GraphNode& b) {
	return std::tie(a.position.x, a.position.y) < std::tie(b.position.x, b.position.y);
}

struct ComparePairLessThan {
	using pair = std::pair<float, GraphNode*>;

	inline bool operator() (const pair& a, const pair& b) const {
		return a.first < b.first;
	}

};

struct Graph : Component {
	explicit Graph(const EntityType type, const EntityID id)
		: Component(type, id) { }

	[[nodiscard]] std::string_view getName() const override {
		return "Graph";
	}

	friend std::ostream& operator<<(std::ostream& os, const Graph& g);

	std::set<GraphNode> nodes;
};