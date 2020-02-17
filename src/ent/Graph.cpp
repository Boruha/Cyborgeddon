#include <ent/Graph.hpp>

#include <algorithm>

//MAPNODE FUNCTIONS
MapNode::MapNode(const float x, const float y) : coord(x, 0, y) {
	connections.reserve(SIZE_CONNECTIONS);
}

bool MapNode::operator!=(const MapNode& m) const {
	return coord != m.coord;
}

bool MapNode::operator==(const MapNode& m) const {
	return coord == m.coord;
}

std::vector<Connection>& MapNode::getConns() {
	return connections;
}


//NODE RECORD FUNCTIONS
bool NodeRecord::operator<(const NodeRecord& c) const {
	return const_so_far < c.const_so_far;
}

void NodeRecord::sortNodeRecord(std::vector<NodeRecord>& records) const {
	std::sort(records.begin(), records.end());
}

bool NodeRecord::contains(const std::vector<NodeRecord>& records, const int node) const {
    for (const auto& rec : records)
        if(rec.node == node)
            return true;

    return false;
} 

const NodeRecord * NodeRecord::find(const std::vector<NodeRecord>& records, const int node) const {
    for (const auto& rec : records)
        if(rec.node == node)
            return &rec;

    return nullptr;
}

NodeRecord * NodeRecord::find(const std::vector<NodeRecord>& records, const int node) {
	return const_cast<NodeRecord *>(std::as_const(*this).find(records, node));
}