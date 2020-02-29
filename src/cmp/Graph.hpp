#pragma once

#include <cmp/Component.hpp>
#include <set>

using glm::vec2;

constexpr unsigned C = 6;

struct GraphNode {

	using pair = std::pair<float, GraphNode*>;

	explicit GraphNode(const vec2& p, char c = '0');

	vec2 position           {         };
	char id                 {         };
	bool alreadyVisited[C]  {         };
	pair conns[C]           {         };
	unsigned numConnections {    0    };
	GraphNode* parent       { nullptr };
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