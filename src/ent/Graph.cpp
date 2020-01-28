#include <ent/Graph.hpp>

//MAPNODE FUNCTIONS
MapNode::MapNode(const float x, const float y) : coord(x,y) { connections.reserve(SIZE_CONNECTIONS); }

void MapNode::addConnection(Connection& new_conn){ connections.emplace_back(new_conn); }


void MapNode::sortConnections(){ std::sort(connections.begin(), connections.end()); }


//CONNECTION FUNCTIONS
bool Connection::operator< (const Connection& c) const { return this->cost < c.cost; }
//bool Connection::operator< (const Connection& c, const Connection& c2) const { return c.cost < c2.cost; }
