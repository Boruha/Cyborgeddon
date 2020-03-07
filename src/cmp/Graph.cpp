#include <cmp/Graph.hpp>
#include <glm/gtx/string_cast.hpp>
#include <ostream>

GraphNode::GraphNode(const vec2 & p, const char c)
	: position(p), id(c)
{
	std::fill(std::begin(alreadyVisited), std::begin(alreadyVisited) + sizeof(alreadyVisited), false);
	std::fill(std::begin(conns), std::begin(conns) + sizeof(conns) / sizeof(pair), pair { INFINITY, nullptr });
}

std::ostream & operator << (std::ostream & os, const Graph & g) {
	g.print(os, "Graph");

	unsigned i = 1;

	for (const auto & gn : g.nodes)
		os << "\n\t" << i++ << ". " << glm::to_string(gn.position);

	return os;
}
