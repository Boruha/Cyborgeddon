#include <ent/Graph.hpp>

//MAPNODE FUNCTIONS
MapNode::MapNode(float x, float y) : coord(x,y) { connections.reserve(SIZE_CONNECTIONS); }

bool MapNode::operator!= (const MapNode& m) const { return this->coord != m.coord; }

bool MapNode::operator== (const MapNode& m) const { return this->coord == m.coord; }

std::vector<Connection>&  MapNode::getConns() { return connections; }


//NODE RECORD FUNCTIONS
bool NodeRecord::operator< (const NodeRecord& c) const { return this->const_so_far < c.const_so_far; }

void NodeRecord::sortNodeRecord(std::vector<NodeRecord>& records) { std::sort(records.begin(), records.end()); }

bool NodeRecord::contains(std::vector<NodeRecord>& records, int node)
{
    bool result = false;

    for(auto& rec : records)
    {
        if(rec.node == node){
            result = true;
            break;
        }
    }
    return result;
} 

NodeRecord* NodeRecord::find(std::vector<NodeRecord>& records, int node)
{
    NodeRecord* result = nullptr;

    for(auto& rec : records)
    {
        if(rec.node == node){
            result = &rec;
            break;
        }
    }
    return result;
}
