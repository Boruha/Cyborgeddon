#include <ent/Graph.hpp>

//MAPNODE FUNCTIONS
MapNode::MapNode(const float x, const float y) : coord(x,y) { connections.reserve(SIZE_CONNECTIONS); }

void MapNode::addConnection(Connection& new_conn){ connections.emplace_back(new_conn); }

//void MapNode::sortConnections(){ std::sort(connections.begin(), connections.end()); }


//CONNECTION FUNCTIONS
//bool operator< (Connection& c1, Connection& c2){ return c1.cost < c2.cost; }
